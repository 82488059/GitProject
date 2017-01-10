# -*- coding: UTF-8 -*-

import urllib2
import time
import datetime
import sys

reload(sys)
sys.setdefaultencoding('utf-8')


def WeatherForecast():
    req = urllib2.urlopen('http://www.weather.com.cn/data/sk/101010100.html', timeout=1000)
    data = req.read()
    print data.encode('gb2312')
    print type(data)
    return data.encode('gb2312')
