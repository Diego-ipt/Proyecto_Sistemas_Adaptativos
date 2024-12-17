import os
import numpy as np
import pandas as pd
import re
import argparse

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
    
    if not os.path.exists(directory):
        print(f"Warning: Directory {directory} does not exist")
        return pd.DataFrame()
        
    for filename in os.listdir(directory):
        if filename.endswith('.salida_0'):
            full_path = os.path.join(directory, filename)
            file_info = parse_filename(filename)
            
            if file_info:
                try:
                    with open(full_path, 'r') as f:
                        lines = f.readlines()
                        
                        # Get the last line (score and time)
                        last_line = lines[-1].strip().split()
                        
                        # Ensure we have two numeric values for score and time
                        if len(last_line) >= 2:
                            score, time = float(last_line[0]), float(last_line[1])
                            file_info['score'] = score
                            file_info['time'] = time
                            results.append(file_info)
                        else:
                            print(f"Invalid last line in {filename}: {last_line}")
                            
                except Exception as e:
                    print(f"Error processing file {filename}: {str(e)}")
                
    return pd.DataFrame(results)

def analyze_results(base_path, output_dir, specific_mode=False, 
                   metaheuristics=None, thresholds=None):
    """
    Analyze results across different metaheuristics and thresholds
    
    :param base_path: Base directory containing results
    :param output_dir: Directory to save output CSVs
    :param specific_mode: If True, uses only the predefined specific directories
    :param metaheuristics: List of metaheuristics to process
    :param thresholds: List of thresholds to process
    """
    base_path = os.path.expanduser(base_path)  # Expand ~ to full home directory path
    
    # Default lists if not provided
    if metaheuristics is None:
        metaheuristics = ['brkga', 'metaheuristica', 'greedy']
    if thresholds is None:
        thresholds = ['th0.75', 'th0.80', 'th0.85']
    
    # Specific predefined directories for quick processing
    specific_dirs = [f'{mh}/{th}_60s' for mh in ['brkga'] for th in thresholds]
    
    # Ensure output directory exists
    os.makedirs(output_dir, exist_ok=True)
    
    all_results = {}
    
    # Choose which directories to process based on mode
    if specific_mode:
        process_dirs = specific_dirs
    else:
        # Generate all possible combinations
        process_dirs = [f'{mh}/{th}' for mh in metaheuristics for th in thresholds]
    
    for dir_subpath in process_dirs:
        dir_path = os.path.join(base_path, dir_subpath)
        
        print(f"Processing {dir_subpath}...")
        
        if os.path.exists(dir_path):
            df = process_results_directory(dir_path)
            
            if not df.empty:
                # Group by N and M, calculate statistics
                grouped = df.groupby(['N', 'M'])
                stats = grouped.agg({
                    'score': ['mean', 'std', 'count'],
                    'time': ['mean', 'std']
                }).reset_index()
                
                # Flatten MultiIndex columns for CSV export
                stats.columns = ['N', 'M', 'score_mean', 'score_std', 'instance_count', 'time_mean', 'time_std']
                
                # Create a filename-safe version of the directory path
                safe_filename = dir_subpath.replace('/', '_').replace('\\', '_')
                
                # Save to CSV
                csv_filename = os.path.join(output_dir, f'{safe_filename}_results.csv')
                stats.to_csv(csv_filename, index=False)
                print(f"Saved results to {csv_filename}")
                
                # Store results
                all_results[dir_subpath] = stats
            else:
                print(f"No valid results found in {dir_subpath}")
        else:
            print(f"Directory not found: {dir_path}")
    
    return all_results

def main():
    """Main function to run analysis and export results"""
    # Set up argument parser
    parser = argparse.ArgumentParser(description='Analyze experimental results')
    parser.add_argument('-b', '--base_path', default=r".\Ejecuciones", 
                        help='Base directory containing results')
    parser.add_argument('-o', '--output_dir', default=r".\results_export", 
                        help='Directory to save output CSVs')
    parser.add_argument('--specific', action='store_true', 
                        help='Process only specific 60s directories for BRKGA')
    
    # Parse arguments
    args = parser.parse_args()
    
    print(f"Processing results from: {os.path.expanduser(args.base_path)}")
    print(f"Saving results to: {args.output_dir}")
    
    try:
        # Run analysis based on specific flag
        results = analyze_results(
            base_path=args.base_path, 
            output_dir=args.output_dir,
            specific_mode=args.specific
        )
        print("\nAnalysis completed successfully!")
        return results
    except Exception as e:
        print(f"An error occurred during analysis: {str(e)}")
        return None

if __name__ == '__main__':
    main()