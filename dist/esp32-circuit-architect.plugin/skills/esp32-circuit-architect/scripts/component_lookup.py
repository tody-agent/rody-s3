#!/usr/bin/env python3
"""
Component Lookup & Curation Utility for ESP32 Circuit Architect
Query, inspect, and append hardware components into the persistent catalog.
"""

import sys
import os
import json
import argparse

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
CATALOG_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "references", "esp32_100_components.json"))

def load_catalog():
    if not os.path.exists(CATALOG_PATH):
        print(f"Error: Catalog not found at {CATALOG_PATH}", file=sys.stderr)
        return []
    with open(CATALOG_PATH, "r", encoding="utf-8") as f:
        return json.load(f)

def save_catalog(data):
    with open(CATALOG_PATH, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
    print(f"Catalog successfully updated! Total components: {len(data)}")

def search_components(catalog, query, category=None):
    results = []
    q = query.lower() if query else ""
    for c in catalog:
        if category and category.lower() not in c["category"].lower():
            continue
        text_match = (
            q in c["id"].lower() or
            q in c["name"].lower() or
            q in c["category"].lower() or
            q in c.get("interface", "").lower() or
            q in c.get("shopee_keywords", "").lower() or
            any(q in p["name"].lower() for p in c.get("pinout", []))
        )
        if text_match:
            results.append(c)
    return results

def print_component(c, detailed=True):
    print(f"\n========================================================")
    print(f"📦 [{c['id']}] {c['name']}")
    print(f"   Category: {c['category']}")
    print(f"   Voltage: {c['operating_voltage']} | Logic: {c['logic_level']}")
    print(f"   Interface: {c['interface']}")
    if c.get("default_address"):
        print(f"   I2C Address: {c['default_address']}")
    print(f"   Typical Current: ~{c['current_draw_typical_ma']} mA")
    print(f"   Shopee Search: {c['shopee_keywords']}")
    
    if detailed:
        print(f"\n📌 Pinout:")
        for p in c.get("pinout", []):
            print(f"   - {p['name']:<12} [{p['type']}]: {p['desc']}")
        
        print(f"\n✅ DOs (Được làm):")
        for do in c.get("dos", []):
            print(f"   - {do}")
            
        print(f"\n❌ DON'Ts (Cấm làm / Nguy cơ cháy nổ):")
        for dont in c.get("donts", []):
            print(f"   - 🛑 {dont}")
            
        print(f"\n⚡ ESP32 Special Notes:")
        print(f"   {c.get('esp32_notes', 'N/A')}")
    print(f"========================================================\n")

def add_component(new_item):
    catalog = load_catalog()
    # Check if id already exists
    for i, c in enumerate(catalog):
        if c["id"] == new_item["id"]:
            print(f"Updating existing component ID: {new_item['id']}")
            catalog[i] = new_item
            save_catalog(catalog)
            return
    catalog.append(new_item)
    save_catalog(catalog)

def main():
    parser = argparse.ArgumentParser(description="ESP32 Component Catalog Query & Curation Tool")
    parser.add_argument("query", nargs="?", default="", help="Search query (name, chip ID, or keyword)")
    parser.add_argument("--category", "-c", help="Filter by category")
    parser.add_argument("--list-categories", action="store_true", help="List all categories")
    parser.add_argument("--count", action="store_true", help="Print total number of components")
    parser.add_argument("--add-json", help="Path to JSON file containing a new component to add")
    
    args = parser.parse_args()
    catalog = load_catalog()
    
    if args.count:
        print(f"Total components in catalog: {len(catalog)}")
        return

    if args.list_categories:
        cats = sorted(list(set(c["category"] for c in catalog)))
        print("\nAvailable Categories:")
        for i, cat in enumerate(cats, 1):
            count = sum(1 for c in catalog if c["category"] == cat)
            print(f"  {i}. {cat} ({count} items)")
        return

    if args.add_json:
        with open(args.add_json, "r", encoding="utf-8") as f:
            new_item = json.load(f)
        add_component(new_item)
        return

    results = search_components(catalog, args.query, args.category)
    if not results:
        print(f"No components found matching '{args.query}'")
        return
        
    print(f"Found {len(results)} matching component(s):")
    if len(results) > 5 and not args.query:
        # Brief listing
        for c in results:
            print(f" - [{c['id']}] {c['name']} ({c['category']})")
        print("\nTip: Specify a query or ID to view full details.")
    else:
        for c in results:
            print_component(c, detailed=True)

if __name__ == "__main__":
    main()
