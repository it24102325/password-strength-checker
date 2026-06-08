/*
 * ██████╗  █████╗ ███████╗███████╗██╗    ██╗ ██████╗ ██████╗ ██████╗
 * ██╔══██╗██╔══██╗██╔════╝██╔════╝██║    ██║██╔═══██╗██╔══██╗██╔══██╗
 * ██████╔╝███████║███████╗███████╗██║ █╗ ██║██║   ██║██████╔╝██║  ██║
 * ██╔═══╝ ██╔══██║╚════██║╚════██║██║███╗██║██║   ██║██╔══██╗██║  ██║
 * ██║     ██║  ██║███████║███████║╚███╔███╔╝╚██████╔╝██║  ██║██████╔╝
 * ╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═╝╚═════╝
 *
 * PASSWORD STRENGTH CHECKER
 * Author    : Ghost
 * Version   : 1.0.0
 * Platform  : Kali Linux / Unix-like systems
 * License   : MIT
 *
 * FEATURES:
 *  - Real-time password strength analysis
 *  - Entropy calculation
 *  - Common password detection
 *  - Detailed security recommendations
 *  - Colored terminal output (ANSI escape codes)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

/* ─── ANSI COLOR CODES ─── */
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define GREEN       "\033[32m"

/* ─── CONSTANTS ─── */
#define MAX_PASSWORD_LEN  256
#define MAX_COMMON_PASS   20

/* ─── STRENGTH LEVELS ─── */
typedef enum {
    VERY_WEAK  = 0,
    WEAK       = 1,
    FAIR       = 2,
    STRONG     = 3,
    VERY_STRONG = 4
} StrengthLevel;

/* ─── PASSWORD ANALYSIS STRUCT ─── */
typedef struct {
    int length;
    int has_lowercase;
    int has_uppercase;
    int has_digits;
    int has_special;
    int has_spaces;
    int lowercase_count;
    int uppercase_count;
    int digit_count;
    int special_count;
    int repeated_chars;
    int sequential_chars;
    double entropy;
    int score;
    StrengthLevel level;
    int is_common;
    char recommendations[10][128];
    int rec_count;
} PasswordAnalysis;

/* ─── COMMON PASSWORDS LIST ─── */
const char *common_passwords[MAX_COMMON_PASS] = {
    "password", "123456", "password123", "admin", "letmein",
    "qwerty", "abc123", "monkey", "1234567890", "password1",
    "iloveyou", "sunshine", "princess", "dragon", "master",
    "shadow", "welcome", "login", "passw0rd", "superman"
};

/* ─── FUNCTION PROTOTYPES ─── */
void print_banner(void);
void print_separator(char ch, int width);
void analyze_password(const char *password, PasswordAnalysis *analysis);
void calculate_entropy(const char *password, PasswordAnalysis *analysis);
void check_patterns(const char *password, PasswordAnalysis *analysis);
void generate_recommendations(const char *password, PasswordAnalysis *analysis);
void display_results(const PasswordAnalysis *analysis, const char *password);
void display_strength_bar(int score);
void display_crack_time(double entropy);
void clear_screen(void);
int  check_common_password(const char *password);

/* ════════════════════════════════════════════════════════════
 *  MAIN FUNCTION
 * ════════════════════════════════════════════════════════════ */
int main(void) {
    char password[MAX_PASSWORD_LEN];
    char choice;
    PasswordAnalysis analysis;

    clear_screen();
    print_banner();

    do {
        printf(CYAN "\n  Enter password to analyze: " RESET);
        
        /* Read password without echoing (basic method) */
        if (fgets(password, sizeof(password), stdin) == NULL) {
            fprintf(stderr, RED "  [ERROR] Failed to read input.\n" RESET);
            return EXIT_FAILURE;
        }

        /* Remove trailing newline */
        size_t len = strlen(password);
        if (len > 0 && password[len - 1] == '\n') {
            password[len - 1] = '\0';
        }

        if (strlen(password) == 0) {
            printf(YELLOW "  [!] Password cannot be empty. Try again.\n" RESET);
            continue;
        }

        /* Reset analysis struct */
        memset(&analysis, 0, sizeof(PasswordAnalysis));

        /* Perform analysis */
        analyze_password(password, &analysis);
        calculate_entropy(password, &analysis);
        check_patterns(password, &analysis);
        generate_recommendations(password, &analysis);

        /* Display results */
        display_results(&analysis, password);

        printf(CYAN "\n  Analyze another password? (y/n): " RESET);
        if(scanf(" %c", &choice) != 1) choice = 'n';
        getchar(); /* consume newline */
        clear_screen();
        print_banner();

    } while (choice == 'y' || choice == 'Y');

    printf(GREEN "\n  Thank you for using Password Strength Checker!\n" RESET);
    printf(DIM "  Stay secure. Stay safe.\n\n" RESET);

    return EXIT_SUCCESS;
}

/* ════════════════════════════════════════════════════════════
 *  PRINT BANNER
 * ════════════════════════════════════════════════════════════ */
void print_banner(void) {
    printf(GREEN BOLD);
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════╗\n");
    printf("  ║                                                      ║\n");
    printf("  ║      🔐   PASSWORD STRENGTH CHECKER  v1.0            ║\n");
    printf("  ║             Cybersecurity Analysis Tool              ║\n");
    printf("  ║                      [Ghost]                         ║\n");
    printf("  ║                                                      ║\n");
    printf("  ╚══════════════════════════════════════════════════════╝\n");
    printf(RESET);
}

/* ════════════════════════════════════════════════════════════
 *  PRINT SEPARATOR
 * ════════════════════════════════════════════════════════════ */
void print_separator(char ch, int width) {
    printf("  ");
    for (int i = 0; i < width; i++) printf("%c", ch);
    printf("\n");
}

/* ════════════════════════════════════════════════════════════
 *  ANALYZE PASSWORD
 * ════════════════════════════════════════════════════════════ */
void analyze_password(const char *password, PasswordAnalysis *analysis) {
    analysis->length = (int)strlen(password);
    analysis->score = 0;

    for (int i = 0; i < analysis->length; i++) {
        char c = password[i];
        if (islower(c)) { analysis->has_lowercase = 1; analysis->lowercase_count++; }
        else if (isupper(c)) { analysis->has_uppercase = 1; analysis->uppercase_count++; }
        else if (isdigit(c)) { analysis->has_digits = 1; analysis->digit_count++; }
        else if (c == ' ')  { analysis->has_spaces = 1; }
        else { analysis->has_special = 1; analysis->special_count++; }
    }

    /* ── Scoring ── */
    /* Length scoring */
    if (analysis->length >= 20)      analysis->score += 30;
    else if (analysis->length >= 16) analysis->score += 25;
    else if (analysis->length >= 12) analysis->score += 20;
    else if (analysis->length >= 8)  analysis->score += 10;
    else if (analysis->length >= 6)  analysis->score += 5;

    /* Character variety scoring */
    if (analysis->has_lowercase) analysis->score += 10;
    if (analysis->has_uppercase) analysis->score += 10;
    if (analysis->has_digits)    analysis->score += 10;
    if (analysis->has_special)   analysis->score += 20;

    /* Multiple special chars bonus */
    if (analysis->special_count >= 3) analysis->score += 10;

    /* Common password penalty */
    analysis->is_common = check_common_password(password);
    if (analysis->is_common) analysis->score -= 40;

    /* Clamp score */
    if (analysis->score < 0)   analysis->score = 0;
    if (analysis->score > 100) analysis->score = 100;

    /* Determine level */
    if      (analysis->score >= 80) analysis->level = VERY_STRONG;
    else if (analysis->score >= 60) analysis->level = STRONG;
    else if (analysis->score >= 40) analysis->level = FAIR;
    else if (analysis->score >= 20) analysis->level = WEAK;
    else                            analysis->level = VERY_WEAK;
}

/* ════════════════════════════════════════════════════════════
 *  CALCULATE ENTROPY (bits)
 * ════════════════════════════════════════════════════════════ */
void calculate_entropy(const char *password, PasswordAnalysis *analysis) {
    (void)password;
    int charset_size = 0;
    if (analysis->has_lowercase) charset_size += 26;
    if (analysis->has_uppercase) charset_size += 26;
    if (analysis->has_digits)    charset_size += 10;
    if (analysis->has_special)   charset_size += 32;
    if (analysis->has_spaces)    charset_size += 1;
    if (charset_size == 0)       charset_size = 1;

    analysis->entropy = analysis->length * log2((double)charset_size);
}

/* ════════════════════════════════════════════════════════════
 *  CHECK PATTERNS (repeated/sequential chars)
 * ════════════════════════════════════════════════════════════ */
void check_patterns(const char *password, PasswordAnalysis *analysis) {
    int len = analysis->length;

    /* Check repeated characters (e.g., "aaaa") */
    for (int i = 0; i < len - 2; i++) {
        if (password[i] == password[i+1] && password[i+1] == password[i+2]) {
            analysis->repeated_chars++;
        }
    }

    /* Check sequential characters (e.g., "abc", "123") */
    for (int i = 0; i < len - 2; i++) {
        if ((password[i+1] == password[i] + 1) && (password[i+2] == password[i] + 2)) {
            analysis->sequential_chars++;
        }
    }

    /* Apply penalties */
    if (analysis->repeated_chars > 0)   analysis->score -= analysis->repeated_chars * 5;
    if (analysis->sequential_chars > 0) analysis->score -= analysis->sequential_chars * 5;

    /* Clamp */
    if (analysis->score < 0)   analysis->score = 0;
    if (analysis->score > 100) analysis->score = 100;

    /* Recalculate level */
    if      (analysis->score >= 80) analysis->level = VERY_STRONG;
    else if (analysis->score >= 60) analysis->level = STRONG;
    else if (analysis->score >= 40) analysis->level = FAIR;
    else if (analysis->score >= 20) analysis->level = WEAK;
    else                            analysis->level = VERY_WEAK;
}

/* ════════════════════════════════════════════════════════════
 *  GENERATE RECOMMENDATIONS
 * ════════════════════════════════════════════════════════════ */
void generate_recommendations(const char *password, PasswordAnalysis *analysis) {
    (void)password;
    analysis->rec_count = 0;

    if (analysis->length < 12)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Use at least 12 characters (16+ recommended)");

    if (!analysis->has_uppercase)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Add uppercase letters (A-Z)");

    if (!analysis->has_lowercase)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Add lowercase letters (a-z)");

    if (!analysis->has_digits)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Include numbers (0-9)");

    if (!analysis->has_special)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Add special characters (!@#$%^&*)");

    if (analysis->is_common)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Avoid common/dictionary passwords");

    if (analysis->repeated_chars > 0)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Avoid repeating characters (aaa, 111)");

    if (analysis->sequential_chars > 0)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Avoid sequential patterns (abc, 123)");

    if (analysis->rec_count == 0)
        strcpy(analysis->recommendations[analysis->rec_count++],
               "Excellent! Your password meets all criteria.");
}

/* ════════════════════════════════════════════════════════════
 *  DISPLAY STRENGTH BAR
 * ════════════════════════════════════════════════════════════ */
void display_strength_bar(int score) {
    int filled = score / 5; /* out of 20 blocks */
    const char *color;

    if      (score >= 80) color = GREEN;
    else if (score >= 60) color = CYAN;
    else if (score >= 40) color = YELLOW;
    else if (score >= 20) color = RED;
    else                  color = RED;

    printf("  Strength  [");
    printf("%s", color);
    for (int i = 0; i < 20; i++) {
        if (i < filled) printf("█");
        else            printf("░");
    }
    printf(RESET "] %s%d%%\n" RESET, color, score);
}

/* ════════════════════════════════════════════════════════════
 *  DISPLAY CRACK TIME ESTIMATE
 * ════════════════════════════════════════════════════════════ */
void display_crack_time(double entropy) {
    /* Assume 10 billion guesses/sec (modern GPU cluster) */
    double guesses = pow(2.0, entropy);
    double seconds = guesses / 1e10;

    printf("  Est. Crack Time (10B guesses/sec): " BOLD);

    if      (seconds < 1)           printf(RED    "Instant\n"           RESET);
    else if (seconds < 60)          printf(RED    "%.1f seconds\n"      RESET, seconds);
    else if (seconds < 3600)        printf(YELLOW "%.1f minutes\n"      RESET, seconds / 60);
    else if (seconds < 86400)       printf(YELLOW "%.1f hours\n"        RESET, seconds / 3600);
    else if (seconds < 2592000)     printf(CYAN   "%.1f days\n"         RESET, seconds / 86400);
    else if (seconds < 31536000)    printf(CYAN   "%.1f months\n"       RESET, seconds / 2592000);
    else if (seconds < 3.15e9)      printf(GREEN  "%.1f years\n"        RESET, seconds / 31536000);
    else                            printf(GREEN  "Centuries+\n"        RESET);
}

/* ════════════════════════════════════════════════════════════
 *  DISPLAY FULL RESULTS
 * ════════════════════════════════════════════════════════════ */
void display_results(const PasswordAnalysis *analysis, const char *password) {
    (void)password;
    const char *level_str;
    const char *level_color;

    switch (analysis->level) {
        case VERY_WEAK:  level_str = "VERY WEAK";   level_color = RED;     break;
        case WEAK:       level_str = "WEAK";         level_color = RED;     break;
        case FAIR:       level_str = "FAIR";         level_color = YELLOW;  break;
        case STRONG:     level_str = "STRONG";       level_color = CYAN;    break;
        case VERY_STRONG:level_str = "VERY STRONG";  level_color = GREEN;   break;
        default:         level_str = "UNKNOWN";      level_color = WHITE;   break;
    }

    printf("\n");
    print_separator('=', 54);
    printf(BOLD "  ANALYSIS RESULTS\n" RESET);
    print_separator('-', 54);

    /* Password Length */
    printf("  Password Length : " BOLD "%d characters\n" RESET, analysis->length);

    /* Strength Bar */
    display_strength_bar(analysis->score);

    /* Strength Level */
    printf("  Strength Level  : " BOLD "%s%s%s\n" RESET, level_color, level_str, RESET);

    /* Entropy */
    printf("  Entropy         : " BOLD "%.2f bits\n" RESET, analysis->entropy);

    /* Crack Time */
    display_crack_time(analysis->entropy);

    /* Common Password Warning */
    if (analysis->is_common) {
        printf("\n  " BG_RED BOLD " ⚠  WARNING: This is a commonly used password! " RESET "\n");
    }

    print_separator('-', 54);
    printf(BOLD "  CHARACTER COMPOSITION\n" RESET);
    print_separator('-', 54);

    printf("  Uppercase  : %s%-3d  %s\n",
           analysis->has_uppercase ? GREEN : RED,
           analysis->uppercase_count,
           analysis->has_uppercase ? "✔" : "✘" RESET);
    printf(RESET);

    printf("  Lowercase  : %s%-3d  %s\n",
           analysis->has_lowercase ? GREEN : RED,
           analysis->lowercase_count,
           analysis->has_lowercase ? "✔" : "✘" RESET);
    printf(RESET);

    printf("  Digits     : %s%-3d  %s\n",
           analysis->has_digits ? GREEN : RED,
           analysis->digit_count,
           analysis->has_digits ? "✔" : "✘" RESET);
    printf(RESET);

    printf("  Special    : %s%-3d  %s\n",
           analysis->has_special ? GREEN : RED,
           analysis->special_count,
           analysis->has_special ? "✔" : "✘" RESET);
    printf(RESET);

    if (analysis->repeated_chars > 0)
        printf("  Repeated patterns  : " RED "%d found\n" RESET, analysis->repeated_chars);

    if (analysis->sequential_chars > 0)
        printf("  Sequential patterns: " RED "%d found\n" RESET, analysis->sequential_chars);

    print_separator('-', 54);
    printf(BOLD "  RECOMMENDATIONS\n" RESET);
    print_separator('-', 54);

    for (int i = 0; i < analysis->rec_count; i++) {
        if (analysis->rec_count == 1 && analysis->score >= 80)
            printf("  " GREEN "✔ %s\n" RESET, analysis->recommendations[i]);
        else
            printf("  " YELLOW "→ %s\n" RESET, analysis->recommendations[i]);
    }

    print_separator('=', 54);
}

/* ════════════════════════════════════════════════════════════
 *  CHECK COMMON PASSWORD
 * ════════════════════════════════════════════════════════════ */
int check_common_password(const char *password) {
    char lower_pass[MAX_PASSWORD_LEN];
    int i;
    for (i = 0; password[i]; i++)
        lower_pass[i] = (char)tolower(password[i]);
    lower_pass[i] = '\0';

    for (int j = 0; j < MAX_COMMON_PASS; j++) {
        if (strcmp(lower_pass, common_passwords[j]) == 0)
            return 1;
    }
    return 0;
}

/* ════════════════════════════════════════════════════════════
 *  CLEAR SCREEN
 * ════════════════════════════════════════════════════════════ */
void clear_screen(void) {
    printf("\033[2J\033[H");
}
