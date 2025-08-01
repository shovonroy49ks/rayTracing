    random_string=$(head /dev/urandom | tr -dc A-Za-z0-9 | head -c 10) # Generates a 10-character alphanumeric string
    git add .
    git commit -m "Commit with random string: $random_string"
    git push
