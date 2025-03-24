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

# 初始化数据矩阵
opa_out_matrix = np.zeros((16, 16))  # 16x16 矩阵

# 创建串口对象
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# 新数据格式正则表达式（匹配示例："x= 0| 1023 1022 ... 23"）
pattern = re.compile(r"x= *(\d+)\|(.+)")

# 图表设置
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
x_grid, y_grid = np.meshgrid(range(16), range(16), indexing="ij")
x_pos = x_grid.flatten()
y_pos = y_grid.flatten()

# 创建颜色映射
norm = Normalize(vmin=0, vmax=4096)  # 保持原有范围
colormap = cm.viridis

def update(frame):
    global opa_out_matrix
    
    while ser.in_waiting > 0:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        match = pattern.match(line)
        
        if match:
            try:
                x_n = int(match.group(1))
                values_str = match.group(2)
                # 转换所有值为整数数组
                values = [int(v) for v in values_str.split()]
                
                # 验证数据完整性
                if len(values) == 16 and 0 <= x_n < 16:
                    # 更新整个x_n行的数据
                    opa_out_matrix[x_n, :] = values
            except (ValueError, IndexError) as e:
                print(f"解析错误: {e} | 原始数据: {line}")

    # 更新3D图形
    ax.clear()
    ax.set_xlim(0, 15)
    ax.set_ylim(0, 15)
    ax.set_zlim(0, 4096)
    ax.set_xlabel("x_n")
    ax.set_ylabel("y_n")
    ax.set_zlabel("OPA_OUT")
    ax.set_title("Real-time 3D Visualization (Compact Format)")
    
    # 创建颜色数组
    colors = colormap(norm(opa_out_matrix.flatten()))
    
    # 绘制3D柱状图
    ax.bar3d(
        x_pos - 0.5,  # 居中显示
        y_pos - 0.5,
        np.zeros(256),
        0.8,  # 柱宽
        0.8,
        opa_out_matrix.flatten(),
        color=colors,
        shade=True
    )

# 使用更快的更新间隔（因为数据更新更快）
ani = FuncAnimation(fig, update, interval=50)

plt.show()
ser.close()