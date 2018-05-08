#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2017-12-15 15:40:03
# @Author  : KlausQiu
# @QQ      : 375235513
# @github  : https://github.com/KlausQIU

from HuobiService import *

'''
Market data API
'''

# 获取KLine
print get_kline('btcusdt', '30min', 300)
