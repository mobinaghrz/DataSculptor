# DataSculptor
DataSculptor

# DataSculptor: Intelligent Data Compression Using Huffman Encoding and Machine Learning

## Overview

DataSculptor is an adaptive data compression tool that combines **Huffman Encoding** with **Machine Learning** to optimize compression efficiency. It learns patterns in files (text, images, etc.) and dynamically adjusts the Huffman tree weights for better compression. This project showcases the power of combining classic algorithms with modern machine learning techniques.

## Features

- **Huffman Encoding**: Optimal encoding based on character frequency.
- **Machine Learning**: Uses LSTM or Markov Chains to predict common patterns in files.
- **Adaptive Compression**: Dynamically adjusts Huffman tree weights based on ML predictions.
- **Efficient Data Structures**: Uses a Huffman tree, trie/hash map, and priority queue (min-heap).

## Data Structures & Algorithms
1. **Huffman Tree**: For optimal encoding based on frequency.
2. **Trie/Hash Map**: To store and retrieve compression mappings efficiently.
3. **Priority Queue (Min Heap)**: For efficient Huffman coding.
4. **Machine Learning**: LSTM or Markov Chains to predict patterns.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/DataSculptor.git
   cd DataSculptor


2. Build the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
