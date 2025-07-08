# Recruitment Task Projects

This repository contains two C++ programs developed as part of a recruitment process.  
Each task focuses on different areas of data processing and manipulation.

---

## 🧾 1. Report Generator

**Description:**  
This program processes time intervals from multiple contracts and generates a report with unified and merged time ranges based on a given reporting window. It converts all time data to Warsaw time (with DST support), handles missing end dates, and ensures all periods are merged properly.

**Key features:**
- Parsing and normalizing timestamps with timezone offsets.
- Warsaw timezone conversion (with DST handling).
- Merging overlapping intervals.
- Output in formatted ISO 8601-style timestamps.

📁 File: `Report generator.cpp`

---

## 🧂 2. Ingredient Consumption

**Description:**  
This application aggregates ingredient usage entries by rounding timestamps to full hours. It then calculates total usage of flour, groats, milk, and eggs per rounded hour.

**Key features:**
- Timestamp normalization (rounding to nearest full hour).
- Grouping and aggregating data by hour.
- Output in neatly formatted table with unit conversions.

📁 File: `ingredient consumption task.cpp`

---

## ⚙️ Technologies Used

- **C++**
- Standard Library (STL)
- Time and string manipulation
- No external libraries required

---

## 📌 Notes

These programs were written individually as part of a recruitment challenge and demonstrate algorithmic thinking, date/time handling, and basic reporting.  

