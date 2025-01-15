#!/bin/bash

# Function to display green text
green_text() {
    echo -e "\e[32m$1\e[0m"
}

# Get the repository name from the git remote URL
repo_name=$(basename -s .git "$(git config --get remote.origin.url)")

# Get the last commit hash and message
last_commit=$(git log -1 --pretty=format:"%h - %s")

# Display repository information
green_text "Repository: $repo_name"
green_text "Last Commit: $last_commit"

# Check git status
echo "Checking git status..."
git_status=$(git status --porcelain)
if [[ -z "$git_status" ]]; then
    green_text "✅ No changes to commit. Exiting."
    exit 0
fi

# Prompt user for commit message
read -p "Enter your commit message: " commit_message
if [[ -z "$commit_message" ]]; then
    echo "Commit message cannot be empty. Exiting."
    exit 1
fi

# Add changes, commit, and push to main
echo "Adding changes..."
git add .
echo "Committing changes..."
git commit -m "$commit_message"
echo "Pushing to main branch..."
git push origin main

# Validate push success
if [[ $? -eq 0 ]]; then
    green_text "🚀 Deployment successful to $repo_name! ✅ 🎉"
else
    echo -e "\e[31m❌ Deployment failed. Please check the error messages above.\e[0m"
    exit 1
fi
