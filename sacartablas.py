import os
import numpy as np
import pandas as pd
import re

def parse_filename(filename):
    """Parse filename to extract N, M, and instance details"""
    match = re.match(r'(\d+)-(\d+)-(\d+)\.txt\.salida_0', filename)
    if match:
        return {
            'N': int(match.group(1)),
            'M': int(match.group(2)),
            'instance': int(match.group(3))
        }
    return None

def process_results_directory(directory):
    """Process all result files in a given directory"""
    results = []
    for filename in os.listdir(directory):
        if filename.endswith('.salida_0'):
            file_info = parse_filename(filename)
            if file_info:
                with open(os.path.join(directory, filename), 'r') as f:
                    last_line = f.readlines()[-1].strip()
                    score, time = map(float, last_line.split())
                    file_info['score'] = score
                    file_info['time'] = time
                    results.append(file_info)
    
    return pd.DataFrame(results)

def analyze_results(base_path, output_dir):
    """
    Analyze results across different metaheuristics and thresholds
    Save results to CSV files
    """
    metaheuristics = ['brkga', 'metaheuristica', 'greedy']
    thresholds = ['th0.75', 'th0.8', 'th0.85']
    
    # Ensure output directory exists
    os.makedirs(output_dir, exist_ok=True)
    
    all_results = {}
    
    for mh in metaheuristics:
        mh_results = {}
        for th in thresholds:
            dir_path = os.path.join(base_path, mh, th)
            if os.path.exists(dir_path):
                df = process_results_directory(dir_path)
                
                # Group by N and M, calculate statistics
                grouped = df.groupby(['N', 'M'])
                stats = grouped.agg({
                    'score': ['mean', 'std'],
                    'time': ['mean', 'std']
                }).reset_index()
                
                # Flatten MultiIndex columns for CSV export
                stats.columns = ['N', 'M', 
                                 'score_mean', 'score_std', 
                                 'time_mean', 'time_std']
                
                # Save to CSV
                csv_filename = os.path.join(output_dir, f'{mh}_{th}_results.csv')
                stats.to_csv(csv_filename, index=False)
                
                mh_results[th] = stats
        
        all_results[mh] = mh_results
    
    return all_results

def main(base_path, output_dir):
    """
    Main function to run analysis and export results
    """
    results = analyze_results(base_path, output_dir)
    print(f"Results exported to {output_dir}")
    return results

# Example usage
if __name__ == '__main__':
    base_path = '~/Ejecuciones/'  # Replace with your actual path
    output_dir = './results_export'  # Directory to save CSV files
    main(base_path, output_dir)