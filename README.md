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
git clone https://github.com/it24102325/password-strength-checker.git
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

<img width="1110" height="832" alt="interface 1" src="https://github.com/user-attachments/assets/34d8aa54-9f2b-4e64-ad64-e0c078571817" />

<img width="847" height="827" alt="interface 2 " src="https://github.com/user-attachments/assets/4e91f513-58cd-423a-82c1-cd52ce53e4a4" />

<img width="807" height="746" alt="interface 3" src="https://github.com/user-attachments/assets/da2e4692-bc52-43c4-a328-a38dc7958dd4" />

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
