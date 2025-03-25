import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import re
import threading
import queue
from matplotlib import cm
from matplotlib.colors import Normalize

# 串口配置
SERIAL_PORT = 'COM20'  # 请根据你的设备修改
BAUD_RATE = 115200     # 波特率

# 初始化 16x16 矩阵
opa_out_matrix = np.zeros((16, 16))

# 创建串口对象
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)

# 数据队列
data_queue = queue.Queue()

# 全局缓冲区
serial_buffer = ''

# 串口读取线程
def serial_reader():
    global serial_buffer
    pattern = re.compile(r'x_n=(\d+):')
    while True:
        try:
            data = ser.read(1024).decode('utf-8', errors='ignore')
            if data:
                serial_buffer += data
                matches = list(pattern.finditer(serial_buffer))
                if not matches:
                    continue

                prev_end = 0
                processed_pos = 0

                for i in range(len(matches)):
                    match = matches[i]
                    if match.start() < prev_end:
                        continue

                    x_n = int(match.group(1))
                    start_idx = match.end()

                    # 确定结束位置
                    if i < len(matches) - 1:
                        end_idx = matches[i+1].start()
                    else:
                        end_idx = len(serial_buffer)

                    # 提取并清理数据
                    data_part = serial_buffer[start_idx:end_idx]
                    data_clean = re.sub(r'[\r\n]', '', data_part)
                    values = []
                    for s in data_clean.split(','):
                        s_clean = s.strip()
                        if s_clean:
                            try:
                                values.append(int(s_clean))
                            except ValueError:
                                pass

                    if values:
                        data_queue.put((x_n, values))
                    
                    processed_pos = end_idx
                    prev_end = processed_pos

                # 截断缓冲区
                serial_buffer = serial_buffer[processed_pos:]

        except Exception as e:
            print(f"串口读取错误: {e}")

threading.Thread(target=serial_reader, daemon=True).start()

# 3D图表设置
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# 生成网格坐标
x_pos, y_pos = np.meshgrid(range(16), range(16), indexing='ij')
x_pos = x_pos.flatten()
y_pos = y_pos.flatten()
dx = dy = np.ones_like(x_pos)  # 修改为1，与第二个代码一致
dz = np.zeros_like(x_pos)

# 颜色映射配置
norm = Normalize(vmin=0, vmax=4096)
colormap = cm.viridis

# 更新函数
def update(frame):
    global opa_out_matrix
    
    # 处理队列数据
    while not data_queue.empty():
        try:
            x_n, values = data_queue.get()
            valid_length = min(len(values), 16)
            opa_out_matrix[x_n, :valid_length] = values[:valid_length]
        except Exception as e:
            print(f"处理数据错误: {e}")

    # 更新3D图形
    ax.clear()
    dz = opa_out_matrix.flatten()
    colors = colormap(norm(dz))
    
    ax.bar3d(x_pos, y_pos, np.zeros_like(dz), 
             dx, dy, dz,
             color=colors,
             shade=True)
    
    # 坐标轴设置，与第二个代码一致
    ax.set_xlim(0, 15)
    ax.set_ylim(0, 15)
    ax.set_zlim(0, 4096)
    ax.set_xlabel('x_n')
    ax.set_ylabel('y_n')
    ax.set_zlabel('OPA_OUT')
    ax.set_title('Real-time 3D Visualization of OPA_OUT')

    return [ax]

# 创建动画，更新间隔改为100ms
ani = FuncAnimation(fig, update, interval=100, blit=False)

plt.show()
ser.close()