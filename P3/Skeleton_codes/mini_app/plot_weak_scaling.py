import sys
import matplotlib.pyplot as plt

def parse_weak_scaling_output(filename):
    results = {}
    current_base = None
    
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            
            parts = line.split(',')
            if len(parts) == 1:
                current_base = int(parts[0])
                results[current_base] = {'threads': [], 'nx': [], 'efficiency': []}
            elif len(parts) == 4 and current_base is not None:
                results[current_base]['threads'].append(int(parts[0]))
                results[current_base]['nx'].append(int(parts[1]))
                results[current_base]['efficiency'].append(float(parts[3]))
    
    return results

def plot_weak_scaling(results):
    fig, ax = plt.subplots(figsize=(10, 7))
    
    ax.axhline(y=1.0, color='k', linestyle='--', label='Ideal')
    
    offsets = [(8, 8), (8, -12), (-35, 8), (-35, -12)]
    
    for idx, base in enumerate(sorted(results.keys())):
        data = results[base]
        line = ax.plot(data['threads'], data['efficiency'], 'o-', 
                      label=f'base={base}', markersize=8)
        color = line[0].get_color()
        
        for i in range(len(data['threads'])):
            ax.annotate(f"{data['nx'][i]}", 
                       (data['threads'][i], data['efficiency'][i]),
                       textcoords="offset points",
                       xytext=offsets[idx],
                       fontsize=8,
                       color=color)
    
    ax.set_xlabel('Number of Processors')
    ax.set_ylabel('Speedup (Efficiency)')
    ax.set_title('Weak Scaling: Speedup vs Number of Processors')
    ax.legend()
    ax.grid(True)
    ax.set_xscale('log', base=2)
    ax.set_ylim(0, 1.2)
    
    plt.tight_layout()
    plt.savefig('weak_scaling.png', dpi=300)
    plt.show()

def main():
    results = parse_weak_scaling_output(sys.argv[1])
    
    print("Weak Scaling Results:")
    for base in sorted(results.keys()):
        print(f"\nBase resolution: {base}")
        print("Threads | nx    | Efficiency")
        print("-" * 35)
        data = results[base]
        for i in range(len(data['threads'])):
            print(f"{data['threads'][i]:7d} | {data['nx'][i]:5d} | {data['efficiency'][i]:10.3f}")
    
    plot_weak_scaling(results)

if __name__ == "__main__":
    main()

