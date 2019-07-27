# GPS_CoordinateTransform
GPS经纬度坐标类，提供了`BD-09`，`GCJ-02`，`WGS-84`以及`MC`墨卡托投影之间的转换。成员变量只有经纬度两个`double`，以便与其他二进制格式兼容。不保存坐标系类型，需要用户自行记录，也因此各转换函数不会做合法性检查。

## 成员变量
`double longitude; // 经度`

`double latitude;  // 纬度`

## 初始化方法
提供默认构造函数、输入两个`double`的构造函数以及使用`std::pair<double,double>`的构造函数。

## 坐标系类型
| 坐标系代号  | 名称                                     | 使用举例                               |
| ------ | ---------------------------------------- | -------------------------------- |
| BD-09  | [百度](https://zh.wikipedia.org/wiki/%E4%B8%AD%E5%8D%8E%E4%BA%BA%E6%B0%91%E5%85%B1%E5%92%8C%E5%9B%BD%E6%B5%8B%E7%BB%98%E9%99%90%E5%88%B6#BD-09)                                       | 百度地图                             |
| GCJ-02 | [国家测绘局](https://zh.wikipedia.org/wiki/%E4%B8%AD%E5%8D%8E%E4%BA%BA%E6%B0%91%E5%85%B1%E5%92%8C%E5%9B%BD%E6%B5%8B%E7%BB%98%E9%99%90%E5%88%B6#GCJ-02)                                    | 高德地图 腾讯搜搜地图 阿里云地图 谷歌地图(国内) GPS设备 |
| WGS-84 | [世界大地测量系统](https://zh.wikipedia.org/zh-hans/%E4%B8%96%E7%95%8C%E5%A4%A7%E5%9C%B0%E6%B5%8B%E9%87%8F%E7%B3%BB%E7%BB%9F) | 谷歌地图(国外)  GPS设备(国外)              |
| MC     | [墨卡托投影](https://zh.wikipedia.org/wiki/%E9%BA%A5%E5%8D%A1%E6%89%98%E6%8A%95%E5%BD%B1%E6%B3%95)                                       | 海图 航路图                           |

## 坐标转换方法
| 原始坐标系 | 目标坐标系 | 方法      |
| ------ | ------ | --------------- |
| GCJ-02 | BD-09  | `GCJ02toBD09`   |
| BD-09  | GCJ-02 | `BD09toGCJ02`   |
| GCJ-02 | WGS-84 | `GCJ02toWGS84`  |
| WGS84  | GCJ-02 | `WGS84toGCJ02`  |
| BD-09  | WGS-84 | `BD09toWGS84`   |
| WGS-84 | BD-09  | `WGS84toBD09`   |
| MC     | BD-09  | `MCtoBD09`      |
| BD-09  | MC     | `MCtoBD09`      |