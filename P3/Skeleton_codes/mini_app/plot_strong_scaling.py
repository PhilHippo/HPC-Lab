import re
import sys
import matplotlib.pyplot as plt

def parse_strong_scaling_output(filename):
    results = {}
    with open(filename, 'r') as f:
        lines = f.readlines()
    
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        if line.startswith("SUMMARY FOR nx="):
            nx = int(re.search(r'nx=(\d+)', line).group(1))
            results[nx] = {'threads': [], 'speedups': []}
            i += 1
            
            while i < len(lines):
                line = lines[i].strip()
                if not line or line.startswith("SUMMARY"):
                    break
                parts = line.split(',')
                if len(parts) == 4:
                    results[nx]['threads'].append(int(parts[0]))
                    results[nx]['speedups'].append(float(parts[2]))
                i += 1
                continue
        i += 1
    return results

def plot_strong_scaling(results):
    fig, ax = plt.subplots(figsize=(10, 7))
    
    max_threads = max([max(data['threads']) for data in results.values()])
    ax.plot([1, max_threads], [1, max_threads], 'k--', label='Ideal')
    
    for nx in sorted(results.keys()):
        data = results[nx]
        ax.plot(data['threads'], data['speedups'], 'o-', label=f'nx={nx}')
    
    ax.set_xlabel('Number of Threads')
    ax.set_ylabel('Speedup')
    ax.set_title('Strong Scaling: Speedup vs Number of Threads')
    ax.legend()
    ax.grid(True)
    ax.set_xscale('log', base=2)
    ax.set_yscale('log', base=2)
    
    plt.tight_layout()
    plt.savefig('strong_scaling.png', dpi=300)
    plt.show()

def main():
    results = parse_strong_scaling_output(sys.argv[1])
    plot_strong_scaling(results)

if __name__ == "__main__":
    main()

