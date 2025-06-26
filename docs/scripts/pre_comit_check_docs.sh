#!/usr/bin/env bash

echo "🔧 Regenerating documentation from source..."
rm README_pc.md
python docs/scripts/inject_code.py docs/user_written_docs/README.md --out README.md

echo "✅ Documentation generation is complete"