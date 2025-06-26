#!/usr/bin/env bash

echo "🔧 Regenerating documentation from source..."
rm README.md
python docs/scripts/inject_code.py docs/user_written_docs/README.md --out README.md
git add README.md
echo "✅ Documentation generation is complete"