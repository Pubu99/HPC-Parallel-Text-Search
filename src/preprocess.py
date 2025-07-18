import pandas as pd

# Load CSV
df = pd.read_csv('../data/IMDB Dataset.csv')

# Clean and write reviews to a txt file (one per line, lowercase)
with open('../data/cleaned_reviews.txt', 'w') as f:
    for review in df['review']:
        # Simple cleaning: remove HTML tags and lower case
        clean = review.replace('<br />', ' ').lower()
        f.write(clean.strip() + '\n')

print("✅ Cleaned reviews written to cleaned_reviews.txt")

