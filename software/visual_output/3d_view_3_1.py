import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import re
from matplotlib import cm
from matplotlib.colors import Normalize

# 串口配置
SERIAL_PORT = 'COM20'  # 修改为你的串口
BAUD_RATE = 115200

# 初始化数据
x_n_values = list(range(16))  # x_n 从 0 到 15
y_n_values = list(range(16))  # y_n 从 0 到 15
opa_out_matrix = np.zeros((16, 16))  # 初始化 OPA_OUT 对应值

# 创建串口对象
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# 正则表达式匹配 x_n, y_n 和 OPA_OUT
pattern = re.compile(r"x_n is (\d+), y_n is (\d+), OPA_OUT=(\d+)")

# 图表设置
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
x_pos, y_pos = np.meshgrid(x_n_values, y_n_values, indexing="ij")
x_pos = x_pos.flatten()
y_pos = y_pos.flatten()
z_pos = np.zeros_like(x_pos)
dx = dy = np.ones_like(x_pos)

# 创建颜色映射
norm = Normalize(vmin=0, vmax=4096)  # 假设 OPA_OUT 的范围是 0 到 4096
colormap = cm.viridis  # 使用 Viridis 颜色映射

# 更新函数
def update(frame):
    global opa_out_matrix

    # 读取串口数据
    while ser.in_waiting:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        match = pattern.search(line)
        if match:
            x_n = int(match.group(1))
            y_n = int(match.group(2))
            opa_out = int(match.group(3))

            # opa_out *= 3

            if 0 <= x_n < 16 and 0 <= y_n < 16:
                opa_out_matrix[x_n, y_n] = opa_out

    # 更新柱状图
    ax.clear()
    ax.set_xlim(0, 15)
    ax.set_ylim(0, 15)
    ax.set_zlim(0, 4096)
    ax.set_xlabel("x_n")
    ax.set_ylabel("y_n")
    ax.set_zlabel("OPA_OUT")
    ax.set_title("Real-time 3D Visualization of OPA_OUT")

    z_pos = np.zeros_like(x_pos)
    dz = opa_out_matrix.flatten()

    # 映射颜色到高度
    colors = colormap(norm(dz))
    ax.bar3d(x_pos, y_pos, z_pos, dx, dy, dz, shade=True, color=colors)

# 动画
ani = FuncAnimation(fig, update, interval=100)  # 每 100ms 更新一次

# 显示图表
plt.show()

# 程序退出时关闭串口
ser.close()
