import pandas as pd
import matplotlib.pyplot as plt

# Load data
data = pd.read_csv("latency_data.txt")

# Plot
plt.figure(figsize=(10, 6))
plt.plot(data["Region_KB"], data["Latency_ns"], marker='o')
plt.xlabel("Memory Region Size (KB)")
plt.ylabel("Latency (ns)")
plt.title("Memory Access Latency vs. Region Size")
plt.grid(True)
plt.tight_layout()

# Save figure
plt.savefig("latency_plot.png")

# Display
plt.show()
