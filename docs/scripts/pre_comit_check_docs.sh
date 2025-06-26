#!/usr/bin/env bash

echo "🔧 Regenerating README_pc.md from source..."
rm README_pc.md
python docs/scripts/inject_code.py docs/user_written_docs/README.md --out README_pc.md

echo "✅ README_pc.md is up to date."