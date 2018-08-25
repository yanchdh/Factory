#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import defaultdict


class EventManager:
  """事件监听管理类"""
  def __init__(self, owner=None):
    self.owner = owner
    self.listenerId = 0
    self.eventDict = defaultdict(dict)
    self.whoDict = defaultdict(set)

  def AddListener(self, evt, listener, who=None):
    """
    添加事件监听函数, 注: listener返回True, 触发事件监听函数时自动删除
    :param evt: 事件类型
    :param listener: 事件监听函数
    :param who: 添加事件监听函数的对象或者标识, 可通过who删除其添加的所有事件
    :return: int: listenerId, 监听函数标识
    """
    self.listenerId += 1
    listenerId = self.listenerId
    self.eventDict[evt][listenerId] = listener
    who and self.whoDict[who].add((evt, listenerId))
    return listenerId

  def Invoke(self, evt, *args, **kwargs):
    """
    触发事件监听函数, listener异常时自动删除
    :param evt: 事件类型
    :param args: listener参数
    :param kwargs: listener参数
    :return:
    """
    listenerDict = self.eventDict.get(evt, None)
    if not listenerDict:
      return
    removeListenerIds = []
    for listenerId in listenerDict:
      try:
        if listenerDict[listenerId](*args, **kwargs):
          removeListenerIds.append(listenerId)
      except Exception as ex:
        removeListenerIds.append(listenerId)
        print("[EventManager] Invoke evt:{!r}, {!r}".format(evt, ex))
    for listenerId in removeListenerIds:
      listenerDict.pop(listenerId)
    not listenerDict and self.eventDict.pop(evt, None)

  def InvokeByListenerId(self, evt, listenerId, *args, **kwargs):
    """
    触发事件监听函数, 指定listenerId
    :param evt: 事件类型
    :param listenerId: 监听函数标识
    :param args: listener参数
    :param kwargs: listener参数
    :return:
    """
    listenerDict = self.eventDict.get(evt, None)
    if not listenerDict or listenerId not in listenerDict:
      return
    try:
      if listenerDict[listenerId](*args, **kwargs):
        listenerDict.pop(listenerId)
    except Exception as ex:
      listenerDict.pop(listenerId)
      print("[EventManager] InvokeByListenerId evt:{!r}, listenerId:{}, {!r}".format(evt, listenerId, ex))
    not listenerDict and self.eventDict.pop(evt, None)

  def RemoveListener(self, evt, listenerId):
    """
    删除事件触发函数
    :param evt: 事件类型
    :param listenerId: 监听函数标识
    :return:
    """
    self.eventDict.get(evt, {}).pop(listenerId, None)

  def RemoveAllListenerByWho(self, who):
    """
    删除通过who添加的所有事件
    :param who: 添加事件监听函数的对象或者标识
    :return:
    """
    listenerSet = self.whoDict.pop(who, None)
    if not listenerSet:
      return
    for evt, listenerId in listenerSet:
      self.eventDict.get(evt, {}).pop(listenerId, None)


######################## simple test ########################
def func(msg, mask=1):
  print('func msg: %s, mask: %s' % (msg, mask))
  return mask == 2

em = EventManager()

id1 = em.AddListener(1, func)
id2 = em.AddListener(1, func)

em.InvokeByListenerId(1, id1, "msg 1", mask=1)
em.Invoke(1, "msg 2", mask=2)
em.Invoke(1, "msg 3", mask=3)
######################## simple test ########################