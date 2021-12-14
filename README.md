# MagicCube 基于ESP32开发的小魔方
  - 视频演示：https://www.bilibili.com/video/BV14r4y1C7Rs
  - 本项目idea参考：稚晖君的 holoCubic https://www.oschina.net/p/holocubic
  - ESP32的主频不要开太高，也用不着那么高（我开的80MHz），240MHz功耗太大，芯片发的热会把热熔胶融化，同时LDO供电也扛不住，实际上我板载的LDO给ESP供电，另外搭了一路LDO给液晶屏供电
  - 程序仅供参考

## 基本功能
### 1. 时钟
    - DS3231高精度时钟模块，断电后持续计时
    - wifi联网后，每隔15min自动从淘宝服务器校准时间，如果DS3231误差超过5秒，同时校准DS3231模块
### 2. 天气预报
    - wifi联网后，每隔30min自动从qweather获取当天以及未来三天的天气预报
### 3. 相册
    - 随机播放相册以及对应语句
    - 每隔1min切换下一张
### 4. 倒计时
	- 以分钟为单位进行倒计时
### 5. 设置
	- 已联网，显示后台管理页面
	- 未联网，自动开启wifi并显示后台管理页面
### 6. 其他
	- 与天猫精灵联动
  
### 7. 参考&&引用
  - Arduino
  - TFT-eSPI
  - ArduinoJson
  - Lvgl
  - RTC Lib

## 食用方法
### 1. 食材
  - ESP32开发板
  - TFT屏
  - DS3212时钟模块（非必须，因为联网会校正时间并启用ESP32内置的RTC模块)
  - MPU6050模块
### 2.如何烹饪
  - 推荐使用 VS code + platform IO插件开发
  - 克隆到本地后，用platform IO插件将工程目录加载进来即可开发，左下方有编译和下载的按钮（具体使用方法请百度）
  - 硬件接线定义在 app.cpp 文件头部注释里，左边对应开发板引脚编号 -- 右边为接线，如 [22]-[I2C_SCL]， 代表开发板上丝印为[22]引脚为[I2C_SCL]引脚，可以连接到MPU6050及DS3212的SCL上
  - tft-espi库里，user_setup_select.h 选择你屏幕对应的驱动，如 ST7789 则取消注释 #include <User_Setups/Setup24_ST7789.h> ，根据你的接线，修改 user_setups文件夹里，Setup24_ST7789.h 修改屏幕对应的相应引脚
  ~~~
	#define TFT_MISO 19
	#define TFT_MOSI 23
	#define TFT_SCLK 18
	#define TFT_CS    5  // Not connected
	#define TFT_DC    26
	#define TFT_RST   33  // Connect reset to ensure display initialises  
  ~~~
   - 主目录下，drivers/display.cpp 这三行代码可以把屏幕刷成白色，具体tft-espi库的使用请百度：
  ~~~
	tft.begin();
	tft.setRotation(4); /* mirror */
	tft.fillScreen(TFT_BLACK);
  ~~~
   - lib目录下为开源的 ArduinoJson、lvgl、RTClib，无法上传上来，请去对应的仓库下载
   - RTClib 进行了修改，在 RTClib.cpp 末尾增加了以下代码
  ~~~
   	uint8_t RTC_DS3231::enable_battery(void)
	{
	  uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, RTCWireBus);
	  status &= 0x0F; // clear bit7
	  write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, status, RTCWireBus);
	  return status;
	}

	uint8_t RTC_DS3231::get_control_status(void)
	{
	  uint8_t status = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, RTCWireBus);
	  return status;
	}
  ~~~
  - RTClib.h 347行 增加接口
  ~~~
    uint8_t enable_battery();
    uint8_t get_control_status();
  ~~~
### 3.关于配置
  - drivers/config.cpp
  ~~~
  	cfg->wifi_ssid  	// wifi名称
    	cfg->wifi_key   	// wifi密码
  	cfg->city_name 		// 和风天气的城市或地区
  	cfg->qweather_key 	// 和风天气的密钥，up主的密钥仅供验证，如果访问太多，UP主自己就没法访问了....所以请自行去 qweather.com 申请（完全免费）并替换成自己的
  ~~~
  - drivers/qweather.cpp
  ~~~
  	#define LOCATION_NANSHAN            "101280604" // 深圳市南山区，地区对应的ID，请在 qweather.com 查询
  ~~~
### 4.关于app组件
  - widget.cpp 组件的定义及实现
  - 组件的定义，可以理解为手机上的一个app
  ~~~
    typedef struct widget_desc_t
    {
        widget_desc_t *prev;        // prev 上一个组件
        widget_desc_t *next;        // next 下一个组件

        uint8_t id;                 // 组件的ID
        String name;                // 名称
        const lv_img_dsc_t *icon;   // icon图标
        widget_fun_t init;          // 初始化函数，启动时会执行一次这个初始化函数，可以对UI界面进行初始化
        widget_fun_t process;       // 运行中函数，每隔 peroid 会调用一次，可以用来刷新界面，或处理自身的任务
        widget_fun_t exit;          // 退出函数，组件退出时会调用一次，可以用来释放资源
        widget_contrl_t control;    // 控制函数，可以通过这个接口，给组件发送事件消息或参数，例如 按键消息
        uint32_t peroid;            // 运行周期，定义组件的调用周期，决定上面的 process 函数多久调用1次
        uint32_t args;              // 附带参数
    } widget_t;
  ~~~ 
  - 组件的控制接口
  ~~~
      gt_err_t add(widget_t *widget);	// 添加组件
      gt_err_t remove(widget_id_t id);  // 移除组件
      gt_err_t launch(widget_id_t id);  // 启动对应ID的组件（通过ID）
      gt_err_t launch(widget_t *widget); // 启动对应的组件（通过指针）
      gt_err_t exit(widget_id_t id);     // 退出对应的组件（通过ID）
      gt_err_t exit(widget_t *widget);   // 退出对应的组件（通过指针）
      gt_err_t control(uint32_t cmd, void* args); // 向运行中的组件发送cmd消息 及 args参数
      int  get_current_widget_id(void);  // 获取当前运行的组件ID
      void launcher_init(void);   // 启动器，你可以理解为主界面，一个特殊的组件
      void launcher_control(uint32_t cmd, void *args);// 向启动器发送cmd消息 及 args参数，比如切换app，退出app等
  ~~~
  - 组件如何运行的
  ~~~
      // main.cpp 定义启动器任务， 每100ms调用一次，用于处理事件消息
      TASK_DECLARE(task_widget, NULL, 100);               
      // app.cpp， 向启动器注册app组件，如时钟，天气...
      clock_init();
      weather_init();
      photo_init();
      inspire_init();
      setting_init();
      // 启动主界面，显示第一个app，并等待按键消息
      widget.launcher_init();
      widget.launcher_control(LAUNCHER_INIT, GT_NULL);
      // 传感器检测位置，并将上、下、左、右的消息发送给组件，由组件进行处理
      void APP::sensor_monitor(void)
      // 如：发送切换上一个app给启动器，启动器会把图标切换到上一个app
      widget.launcher_control(LAUNCHER_PREV, GT_NULL);
      // 如：发送向左按键消息给当前运行的app，app收到后可以进行界面切换之类的操作
      widget.control(KEY_LEFT, GT_NULL);
  ~~~
### 5.关于天猫精灵的接入
  - 请前往 巴法云 申请密钥（免费），并替换代码中的密钥，UP主的密钥仅供参考，请勿直接使用UP主的密钥...
  - 控制方法请参考代码
  
### 6.关于图像、字体
  - 通过 lvgl 官网的在线图片转换工具、字体转换工具
  - 通过其他第三方字体生成工具，具体请百度下载.
  
### 7.其他
  - UP主因工作繁忙，不一定能及时解答，请大家先参考代码，或百度自行解决
