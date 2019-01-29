# -*- coding: utf-8 -*-

import time
import math


def linear_interpolator(v1, v2, t, interval):
	# 线性插值，可以进行[+ - * /]运算的数值类型
	return v1 + (v2 - v1) * t / interval


def linear_interpolator_vector(v1, v2, t, interval):
	# 线性插值，向量
	return [linear_interpolator(x, y, t, interval) for x, y in zip(v1, v2)]


def linear_interpolator_degree(v1, v2, t, interval):
	# 线性插值，角度
	if v1 < 0:
		v1 += 360
	if v2 < 0:
		v2 += 360
	if v1 - v2 > 180:
		v2 += 360
	if v2 - v1 > 180:
		v1 += 360
	v = linear_interpolator(v1, v2, t, interval)
	if v >= 360:
		v -= 360
	return v if v <= 180 else v - 360


def linear_interpolator_euler(v1, v2, t, interval):
	# 线性插值，欧拉角
	roll = linear_interpolator_degree(v1[0], v2[0], t, interval)
	pitch = linear_interpolator(v1[1], v2[1], t, interval)
	yaw = linear_interpolator_degree(v1[2], v2[2], t, interval)
	return roll, pitch, yaw


class Interpolator(object):
	def __init__(self, interpolator_func=linear_interpolator, sync_interval=0.1, max_sync_delay=0.5):
		# 插值函数
		self.interpolator_func = interpolator_func
		# 同步间隔
		self.sync_interval = sync_interval
		# 同步数据缓存
		self.sync_buff = []
		# 同步数据计数
		self.sync_buff_count = 0
		# sync_buff最大长度
		self.max_sync_buff_size = math.ceil(max_sync_delay / sync_interval) + 1

		# 上次tick时间戳
		self.last_tick_stamp = 0
		# tick最大加速度
		self.tick_max_accelerate = 1.5
		# tick总次数
		self.tick_total_times = 0
		# tick失败次数(未插值次数)
		self.tick_fail_times = 0

		# 最小延时
		self.min_sync_delay = self.sync_interval
		# 最大延时
		self.max_sync_delay = max_sync_delay
		# 上次sync时间戳
		self.last_sync_stamp = 0
		# 同步延时
		self.sync_delay = self.min_sync_delay
		# 最近最大同步延时
		self.latest_max_sync_delay = self.min_sync_delay

		# 是否正在插值
		self.in_interpolating = False
		# 插值时间
		self.interpolator_time = 0
		# 未插值时间
		self.uninterpolator_time = 0

	def sync(self, v):
		""" 同步数据 """
		self.sync_buff.append(v)
		self.sync_buff_count += 1
		sync_buff_size = len(self.sync_buff)
		if sync_buff_size > self.max_sync_buff_size:
			self.sync_buff.pop(0)
			self.interpolator_time = 0
		self._update_sync_delay()

	def tick(self, delta_time):
		""" 返回插值数据 """
		self.last_tick_stamp = time.time()
		self.tick_total_times += 1

		if not self.sync_buff:
			return None

		sync_buff_size = len(self.sync_buff)
		if sync_buff_size == 1:
			self.uninterpolator_time += delta_time
			self.tick_fail_times += 1
			return None

		if not self.in_interpolating:
			if not self._can_interpolator():
				self.uninterpolator_time += delta_time
				self.tick_fail_times += 1
				return None
			self.in_interpolating = True
			self.interpolator_time = 0
			self.uninterpolator_time = 0

		# 加速
		accelerate = self._calc_accelerate()
		delta_time *= accelerate
		self.interpolator_time += delta_time

		while self.interpolator_time >= self.sync_interval and sync_buff_size > 1:
			self.interpolator_time -= self.sync_interval
			self.sync_buff.pop(0)
			sync_buff_size -= 1

		if sync_buff_size == 1:
			self.interpolator_time = 0
			return self.sync_buff[0]

		src, dst = self.sync_buff[0], self.sync_buff[1]
		return self.interpolator_func(src, dst, self.interpolator_time, self.sync_interval)

	def _update_sync_delay(self):
		""" 返回插值数据 """
		sync_stamp = time.time()
		cur_sync_delay = sync_stamp - self.last_sync_stamp
		self.last_sync_stamp = sync_stamp

		if cur_sync_delay < self.max_sync_delay:
			self.latest_max_sync_delay = max(self.latest_max_sync_delay, cur_sync_delay)
		if self.sync_buff_count % 10 == 0:
			assert self.latest_max_sync_delay >= self.sync_interval, '%.2f' % self.latest_max_sync_delay
			# print '****** update_sync_delay %.2f %.2f' % (self.sync_delay, self.latest_max_sync_delay), self
			self.sync_delay = self.latest_max_sync_delay
			self.latest_max_sync_delay = self.min_sync_delay

	def _calc_accelerate(self):
		# sync_buff累积, 计算tick加速度
		accelerate = 1.0
		sync_buff_size = len(self.sync_buff)
		left_time = sync_buff_size / 10.0 - self.interpolator_time
		if left_time - self.sync_delay > self.sync_interval:
			accelerate = min(1.5, left_time / self.sync_delay)
			# print '###### calc_accelerate %d %.2f %.2f %.1f' % (sync_buff_size, left_time, self.sync_delay, accelerate)
		return accelerate

	def _can_interpolator(self):
		""" 是否可以插值 """
		sync_buff_size = len(self.sync_buff)
		return sync_buff_size >= 2 and (sync_buff_size - 1) / 10.0 + self.uninterpolator_time >= self.sync_delay

	def __str__(self):
		return 'Interpolator(%s;%.2f;%d;%d)' % (len(self.sync_buff), self.sync_delay, self.tick_total_times, self.tick_fail_times)


def test():
	import random
	random.seed(2)

	test_time = 5
	delta_time = 0.0167
	v = 0

	ip = Interpolator()
	ip.sync(v)
	v = 1 - v
	ip.sync(v)
	v = 1 - v

	delay_list = [(0.1 * 1, 0.4 * 1), (0.1 * 1, 0.1 * 1), (0.1 * 1, 0.1 * 3), (0.1 * 1, 0.1 * 2), (0.1 * 1, 0.1 * 1)]

	st = time.time()
	t1 = st
	t2 = st + test_time * len(delay_list)
	dt = st

	delay_idx = 0
	delay = delay_list[delay_idx]

	delay_time = t1 + test_time

	n = 0
	rt = st + random.uniform(*delay)

	while t1 < t2:
		if t1 >= delay_time:
			delay_idx += 1
			if delay_idx >= len(delay_list):
				break
			delay_time += test_time
			delay = delay_list[delay_idx]

		if t1 >= rt:
			ip.sync(v)
			v = 1 - v
			n += 1
			rt = st + n * 0.1 + random.uniform(*delay)

		val = ip.tick(delta_time)
		if val is not None:
			print '%.2f' % val, ip, time.time()
		else:
			print val, ip, time.time()

		t1 = time.time()
		dt += delta_time
		time.sleep(dt - t1)


if __name__ == '__main__':
	# test()
	print linear_interpolator_degree(121, -170, 0.1, 1)
