import matplotlib.pyplot as plt
from datetime import datetime
import json

file_path = "logs/client_multi_100ms.log"
timestamps = []
out1_values = []
out2_values = []
out3_values = []

with open(file_path, 'r') as file:
    for line in file:
        # Parse each line as JSON
        data_point = json.loads(line.strip())
        timestamp = datetime.fromtimestamp(data_point['timestamp'] / 1000.0)
        timestamps.append(timestamp)
        out1_values.append(float(data_point['out1']) if data_point['out1'] != '--' else None)
        out2_values.append(float(data_point['out2']) if data_point['out2'] != '--' else None)
        out3_values.append(float(data_point['out3']) if data_point['out3'] != '--' else None)

# Plot the data in subplots
fig, axs = plt.subplots(3, 1, figsize=(10, 15))

# Plot Output 1
axs[0].plot(timestamps, out1_values, marker='o', color='blue')
axs[0].set_ylabel('Output 1')
axs[0].set_title('Output Values Over Time')

# Plot Output 2
axs[1].plot(timestamps, out2_values, marker='o', color='green')
axs[1].set_ylabel('Output 2')

# Plot Output 3
axs[2].plot(timestamps, out3_values, marker='o', color='red')
axs[2].set_ylabel('Output 3')
axs[2].set_xlabel('Timestamp')

# Rotate x-axis labels for better readability
for ax in axs:
    ax.grid(True)
    ax.tick_params(axis='x', rotation=45)

plt.tight_layout()
plt.savefig('server_outs.png')
