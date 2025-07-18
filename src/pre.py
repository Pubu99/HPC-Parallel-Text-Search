import pandas as pd
import os

# Load IMDB dataset
df = pd.read_csv('../data/IMDB Dataset.csv')

# Clean reviews
cleaned_reviews = [review.replace('<br />', ' ').lower().strip() for review in df['review']]

# Split into 10 parts
total_reviews = len(cleaned_reviews)
chunk_size = total_reviews // 10
output_dir = '../data/split_reviews/'
os.makedirs(output_dir, exist_ok=True)

for i in range(10):
    start = i * chunk_size
    end = (i + 1) * chunk_size if i < 9 else total_reviews  # last chunk includes remainder
    filename = os.path.join(output_dir, f'reviews_part_{i+1}.txt')
    with open(filename, 'w') as f:
        for line in cleaned_reviews[start:end]:
            f.write(line + '\n')

print("✅ Split into 10 files in:", output_dir)

