import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import re
from matplotlib import cm
from matplotlib.colors import Normalize

# 串口配置
SERIAL_PORT = 'COM20'  # 请根据你的设备修改
BAUD_RATE = 115200     # 波特率

# 初始化 16x16 矩阵
opa_out_matrix = np.zeros((16, 16))

# 创建串口对象
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# 正则表达式，用于匹配格式 x_n=<数字>:<16个逗号分隔的数字>
pattern = re.compile(r"x_n=([\d]+):([\d,]+)")

# 图表设置
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
x_pos, y_pos = np.meshgrid(range(16), range(16), indexing="ij")
x_pos = x_pos.flatten()
y_pos = y_pos.flatten()
z_pos = np.zeros_like(x_pos)
dx = dy = np.ones_like(x_pos)

# 颜色映射
norm = Normalize(vmin=0, vmax=4096)  # OPA_OUT 的值范围假设为 0-4096
colormap = cm.viridis

# 更新函数
def update(frame):
    global opa_out_matrix

    # 读取串口数据
    while ser.in_waiting:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        match = pattern.search(line)
        if match:
            try:
                x_n = int(match.group(1))           # 提取 x_n
                opa_out_str = match.group(2)        # 提取逗号分隔的数字字符串
                opa_out_list = opa_out_str.split(',')  # 拆分为列表

                # 检查数据完整性
                if 0 <= x_n < 16 and len(opa_out_list) == 16:
                    # 将字符串列表转换为整数列表
                    opa_out_values = [int(val) for val in opa_out_list]
                    # 更新矩阵的对应行
                    opa_out_matrix[x_n, :] = opa_out_values
                else:
                    continue  # 丢弃不符合条件的行
            except ValueError:
                continue  # 丢弃包含非数字的行

    # 更新 3D 柱状图
    ax.clear()
    ax.set_xlim(0, 15)
    ax.set_ylim(0, 15)
    ax.set_zlim(0, 4096)
    ax.set_xlabel("x_n")
    ax.set_ylabel("y_n")
    ax.set_zlabel("OPA_OUT")
    ax.set_title("Real-time 3D Visualization of OPA_OUT")

    dz = opa_out_matrix.flatten()
    colors = colormap(norm(dz))
    ax.bar3d(x_pos, y_pos, z_pos, dx, dy, dz, shade=True, color=colors)

# 创建动画
ani = FuncAnimation(fig, update, interval=100)  # 每 100ms 更新一次

# 显示图表
plt.show()

# 关闭串口
ser.close()