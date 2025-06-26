#!/usr/bin/env bash

echo "🔧 Regenerating README_pc.md from source..."
python docs/scripts/inject_code.py docs/user_written_docs/README.md --out README_pc.md

# Check if the file has changed
if ! git diff --quiet README_pc.md; then
  echo "❌ README_pc.md is out of date. Please commit the updated file."
  exit 1
fi

echo "✅ README_pc.md is up to date."