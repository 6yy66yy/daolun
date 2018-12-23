var myCharts = require('../../../utils/wxcharts.js')//引入一个绘图的插件
var lineChart_hum = null
var lineChart_light = null
var lineChart_tempe = null
var app = getApp()
var timer
var o=1
Page({
  data: {
    time:1,
    countDownNum:1,
    
  },
  
  getDataFromOneNet: function () {
    //从oneNET请求我们的Wi-Fi气象站的数据
    var that = this
    const requestTask = wx.request({
      url: 'https://api.heclouds.com/devices/505412621/datapoints?datastream_id=Number&limit=15&duration=60&sort=DESC',
      header: {
        'content-type': 'application/json',
        'api-key': '1S5FqZzWNK6WuFBJZ8Xl0YOMBho='
      },
      success: function (res) {
        console.log(res.data)
        //拿到数据后保存到全局数据
        app.globalData.Number = []
        app.globalData.Number = res.data.data.datastreams[0]
        console.log(app.globalData.Number)
        //跳转到天气页面，根据拿到的数据绘图
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      }
    })
  },
  onPullDownRefresh: function () {
    console.log('onPullDownRefresh', new Date())
  },
  refresh: function () {
    var that=this
    that.getDataFromOneNet();
    that.onLoad();
  },
  //把拿到的数据转换成绘图插件需要的输入格式
  convert: function () {
    
    var categories = [];
    var Number = [];
    var time = (this.data.time);
    console.log(app.globalData.Number)
    var length = app.globalData.Number.datapoints.length
    for (var i = 0; i < length; i += time) {
      categories.push(app.globalData.Number.datapoints[i].at.slice(11,19));
      Number.push(app.globalData.Number.datapoints[i].value);
    }
    
    return {
      categories: categories,
      Number: Number
    }
    
  },

  onLoad: function () {
    var wheatherData = this.convert();
    //得到屏幕宽度
    var windowWidth = 320;
    try {
      var res = wx.getSystemInfoSync();
      windowWidth = res.windowWidth;
    } catch (e) {
      console.error('getSystemInfoSync failed!');
    }

    var wheatherData = this.convert();

    //新建湿度图表
    lineChart_hum = new myCharts({
      canvasId: 'humidity',
      type: 'line',
      categories: wheatherData.categories,
      animation: true,
      background: '#f5f5f5',
      series: [{
        name: 'Number',
        data: wheatherData.Number,
        format: function (val, name) {
          return val.toFixed(2);
        }
      }],
      xAxis: {
        disableGrid: true
      },
      yAxis: {
        title: 'Number',
        format: function (val) {
          return val.toFixed(2);
        },
        min: 55
      },
      width: windowWidth,
      height: 200,
      dataLabel: false,
      dataPointShape: true,
      extra: {
        lineStyle: 'curve'
      }
    });

//   //新建光照强度图表
//     lineChart_light = new myCharts({
//       canvasId: 'light',
//       type: 'line',
//       categories: wheatherData.categories,
//       animation: true,
//       background: '#f5f5f5',
//       series: [{
//         name: 'light',
//         data: wheatherData.light,
//         format: function (val, name) {
//           return val.toFixed(2);
//         }
//       }],
//       xAxis: {
//         disableGrid: true
//       },
//       yAxis: {
//         title: 'light (lux)',
//         format: function (val) {
//           return val.toFixed(2);
//         },
//         min: 190
//       },
//       width: windowWidth,
//       height: 200,
//       dataLabel: false,
//       dataPointShape: true,
//       extra: {
//         lineStyle: 'curve'
//       }
//     });

//     //新建温度图表
//     lineChart_tempe = new myCharts({
//       canvasId: 'tempe',
//       type: 'line',
//       categories: wheatherData.categories,
//       animation: true,
//       background: '#f5f5f5',
//       series: [{
//         name: 'temperature',
//         data: wheatherData.tempe,
//         format: function (val, name) {
//           return val.toFixed(2);
//         }
//       }],
//       xAxis: {
//         disableGrid: true
//       },
//       yAxis: {
//         title: 'temperature (摄氏度)',
//         format: function (val) {
//           return val.toFixed(2);
//         },
//         min: 24
//       },
//       width: windowWidth,
//       height: 200,
//       dataLabel: false,
//       dataPointShape: true,
//       extra: {
//         lineStyle: 'curve'
//       }
//     });
   },
  onShow: function () {
    //什么时候触发倒计时，就在什么地方调用这个函数
    if(o==1)
    {
    let that = this;
    that.getDataFromOneNet()
    this.countDown();
    o++  ;
  }},
  countDown: function () {
    let that = this;
    let countDownNum = that.data.countDownNum;//获取倒计时初始值
    //如果将定时器设置在外面，那么用户就看不到countDownNum的数值动态变化，所以要把定时器存进data里面
    that.setData({
      timer: setInterval(function () {//这里把setInterval赋值给变量名为timer的变量
        //每隔一秒countDownNum就减一，实现同步
        countDownNum--;
        //然后把countDownNum存进data，好让用户知道时间在倒计着
        that.setData({
          countDownNum: countDownNum
        })
        //在倒计时还未到0时，这中间可以做其他的事情，按项目需求来
        if (countDownNum <= 0) {
          //这里特别要注意，计时器是始终一直在走的，如果你的时间为0，那么就要关掉定时器！不然相当耗性能
          //因为timer是存在data里面的，所以在关掉时，也要在data里取出后再关闭
          countDownNum = 10;
          that.getDataFromOneNet();
          that.onLoad();
          //关闭定时器之后，可作其他处理codes go here
        }
      }, 1000)
    })
  },

  
})
