# VSG算法四象限逆变器

## TARGET:  
* 直流模式：
L正N负，支持单口恒压式或双口低压式能量双向流动  
APF不工作  

* VSG模式：
自适应频率，无需区分离并网  
空闲中预同步锁相->直接启动  
离网已启动->并网:跳保护，之后恢复并网  
并网->离网:没有影响  
APF可选开关  


## TOLIST:  
* 息屏休眠与运输模式
* 环路以外基础功能
* 发波、中断
* 卡采样点，校准ADC
* 保护功能测试，报警功能
* 开环SPWM
* 单电流环
* APF
* 无VSG双环
* VSG
* 直流模式

UI_PAGE_TEXT中：
UI_ITEM_RETURN必须搭配菜单页面
UI_ITEM_DATA是正常的数据项
UI_ITEM_WORD可以呼出文本框
UI_ITEM_PARENTS可以指向本页头部
