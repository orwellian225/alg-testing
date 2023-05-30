#! bin/sh

python util/graph_generator.py 3 data/OPT_BC.csv data/OPT_WC.csv data/OPT_RANDOM.csv "Optimised Algorithm Case Comparison"
python util/graph_generator.py 3 data/BF_BC.csv data/BF_WC.csv data/BF_RANDOM.csv "Brute Force Algorithm Case Comparison"
python util/graph_generator.py 2 data/OPT_BC.csv data/BF_BC.csv "Brute Force vs Optimised Algorithm Best Case"
python util/graph_generator.py 2 data/OPT_WC.csv data/BF_WC.csv "Brute Force vs Optimised Algorithm Worst Case"
python util/graph_generator.py 2 data/OPT_RANDOM.csv data/BF_RANDOM.csv "Brute Force vs Optimised Algorithm Random Case"