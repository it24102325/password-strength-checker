# 🔐 Password Strength Checker

> A cybersecurity tool written in C that analyzes password strength with real-time feedback, entropy calculation, and actionable recommendations.

![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Kali-blue)
![Language](https://img.shields.io/badge/language-C-brightgreen)
![License](https://img.shields.io/badge/license-MIT-orange)

---

## ✨ Features

- **Strength Score** (0–100) with visual progress bar
- **Shannon Entropy** calculation (bits of randomness)
- **Estimated Crack Time** based on 10B guesses/sec (GPU cluster)
- **Character Composition** breakdown (uppercase, lowercase, digits, special)
- **Pattern Detection** — flags repeated & sequential characters
- **Common Password Detection** — warns against dictionary passwords
- **Actionable Recommendations** to improve your password
- **Colorful Terminal UI** using ANSI escape codes

---

## 🛠️ Installation

### Requirements
- GCC (GNU Compiler Collection)
- Make
- Linux / Kali Linux

```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/password-strength-checker.git
cd password-strength-checker

# Build
make

# Run
./password_checker
```

---

## 📊 Strength Levels

| Score | Level       | Color  |
|-------|-------------|--------|
| 0–19  | Very Weak   | 🔴 Red |
| 20–39 | Weak        | 🔴 Red |
| 40–59 | Fair        | 🟡 Yellow |
| 60–79 | Strong      | 🔵 Cyan |
| 80–100| Very Strong | 🟢 Green |

---

## 🖥️ Screenshot

```
╔══════════════════════════════════════════════════════╗
║    🔐  PASSWORD STRENGTH CHECKER  v1.0              ║
╚══════════════════════════════════════════════════════╝

  Enter password to analyze: ••••••••••••••

  ══════════════════════════════════════════════════════
  ANALYSIS RESULTS
  ──────────────────────────────────────────────────────
  Password Length : 16 characters
  Strength  [████████████████░░░░] 80%
  Strength Level  : VERY STRONG
  Entropy         : 104.89 bits
  Est. Crack Time : Centuries+
```

---

## 🔒 How Scoring Works

| Criteria          | Points |
|-------------------|--------|
| Length ≥ 20       | +30    |
| Length ≥ 16       | +25    |
| Length ≥ 12       | +20    |
| Lowercase letters | +10    |
| Uppercase letters | +10    |
| Digits            | +10    |
| Special chars     | +20    |
| 3+ special chars  | +10    |
| Common password   | −40    |
| Repeated patterns | −5 each|

---

## 📁 Project Structure

```
password-strength-checker/
├── password_checker.c   # Main source code
├── Makefile             # Build configuration
├── README.md            # Project documentation
└── .gitignore           # Git ignore rules
```

---

## 🧠 Entropy Formula

```
Entropy = L × log₂(R)
```
- `L` = password length
- `R` = character pool size (26 + 26 + 10 + 32 = 94 for full ASCII)

---

## 🤝 Contributing

Pull requests are welcome! For major changes, please open an issue first.

---

## 📄 License

MIT License — see [LICENSE](LICENSE) for details.

---

*Built for educational purposes. Always use a password manager for real security.*
