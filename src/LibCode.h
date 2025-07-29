/*

#include <stdio.h>
#include <string.h>

void xor_encrypt_decrypt(char *input, char *output, char *key) {
    for (int i = 0; i < strlen(input); i++) {
        output[i] = input[i] ^ key[i % strlen(key)];
    }
    output[strlen(input)] = '\0';
}

int main() {
    char message[] = "HelloWorld";
    char key[] = "secret";
    char encrypted[100];
    char decrypted[100];

    xor_encrypt_decrypt(message, encrypted, key);
    printf("Encrypted: ");
    for (int i = 0; i < strlen(message); i++) {
        printf("%02X ", (unsigned char)encrypted[i]);
    }
    printf("\n");

    xor_encrypt_decrypt(encrypted, decrypted, key);
    printf("Decrypted: %s\n", decrypted);

    return 0;
}

#include <stdio.h>
#include <math.h>

// Einfache modexp-Funktion (Exponentiation by squaring)
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1; // exp /= 2
        base = (base * base) % mod;
    }
    return result;
}

// Berechne ggT
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Berechne Multiplikatives Inverses von e mod phi(n)
int modinv(int e, int phi) {
    for (int d = 1; d < phi; d++) {
        if ((e * d) % phi == 1)
            return d;
    }
    return -1; // Kein Inverses gefunden
}

int main() {
    // Kleine Primes
    int p = 11, q = 13;
    int n = p * q;            // 143
    int phi = (p - 1) * (q - 1); // 120
    int e = 7; // öffentlich

    if (gcd(e, phi) != 1) {
        printf("Ungültiger e-Wert\n");
        return 1;
    }

    int d = modinv(e, phi); // privat

    printf("Public key: (e=%d, n=%d)\n", e, n);
    printf("Private key: (d=%d, n=%d)\n", d, n);

    int plaintext = 42;
    int ciphertext = mod_exp(plaintext, e, n);
    int decrypted = mod_exp(ciphertext, d, n);

    printf("Plaintext: %d\n", plaintext);
    printf("Encrypted: %d\n", ciphertext);
    printf("Decrypted: %d\n", decrypted);

    return 0;
}


#include <stdio.h>
#include <gmp.h>

int main() {
    mpz_t p, q, n, phi, e, d, m, c, decrypted;

    // Initialisieren
    mpz_inits(p, q, n, phi, e, d, m, c, decrypted, NULL);

    // Beispiel: Große Primzahlen (nur zu Demo-Zwecken, in der Praxis generieren!)
    mpz_set_str(p, "61", 10);
    mpz_set_str(q, "53", 10);

    // n = p * q
    mpz_mul(n, p, q);

    // phi = (p-1)*(q-1)
    mpz_t p1, q1;
    mpz_inits(p1, q1, NULL);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    // e = 17
    mpz_set_ui(e, 17);

    // d = e^(-1) mod phi
    mpz_invert(d, e, phi);

    // Nachricht m = 42
    mpz_set_ui(m, 42);

    // Verschlüsseln: c = m^e mod n
    mpz_powm(c, m, e, n);

    // Entschlüsseln: decrypted = c^d mod n
    mpz_powm(decrypted, c, d, n);

    // Ausgabe
    gmp_printf("Public Key (e, n): (%Zd, %Zd)\n", e, n);
    gmp_printf("Private Key (d, n): (%Zd, %Zd)\n", d, n);
    gmp_printf("Original: %Zd\n", m);
    gmp_printf("Encrypted: %Zd\n", c);
    gmp_printf("Decrypted: %Zd\n", decrypted);

    // Aufräumen
    mpz_clears(p, q, n, phi, e, d, m, c, decrypted, p1, q1, NULL);
    return 0;
}
*/


#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <time.h>

// Konvertiert String → mpz_t
void string_to_mpz(mpz_t result, const char *str) {
    mpz_import(result, strlen(str), 1, 1, 0, 0, str);
}

// mpz_t → String
void mpz_to_string(char *output, mpz_t input) {
    size_t count;
    mpz_export(output, &count, 1, 1, 0, 0, input);
    output[count] = '\0';
}

// Zufällige Primzahl mit n Bits generieren
void generate_large_prime(mpz_t prime, gmp_randstate_t state, int bits) {
    do {
        mpz_urandomb(prime, state, bits);
        mpz_setbit(prime, bits - 1); // Stelle höchstes Bit sicher (damit es n Bit hat)
        mpz_nextprime(prime, prime); // Nächste Primzahl >= Zufallszahl
    } while (mpz_sizeinbase(prime, 2) < bits);
}

int main() {
    // Initialisieren
    mpz_t p, q, n, phi, e, d, m, c, decrypted;
    mpz_inits(p, q, n, phi, e, d, m, c, decrypted, NULL);

    gmp_randstate_t state;
    gmp_randinit_mt(state); // Mersenne Twister
    gmp_randseed_ui(state, time(NULL)); // Seed mit Uhrzeit

    int prime_bits = 1024; // 2 × 1024 = 2048-bit modulus

    printf("🔐 Generiere zwei %d-bit Primzahlen...\n", prime_bits);
    generate_large_prime(p, state, prime_bits);
    generate_large_prime(q, state, prime_bits);

    // Modulus n = p * q
    mpz_mul(n, p, q);

    // phi(n) = (p - 1)(q - 1)
    mpz_t p1, q1;
    mpz_inits(p1, q1, NULL);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    // Öffentlicher Exponent e = 65537
    mpz_set_ui(e, 65537);

    // Privater Exponent d = e⁻¹ mod phi(n)
    if (mpz_invert(d, e, phi) == 0) {
        printf("❌ Fehler: Kein Inverses für e mod phi(n).\n");
        return 1;
    }

    // Text → mpz
    const char *plaintext = "Hello, world!";
    string_to_mpz(m, plaintext);

    if (mpz_cmp(m, n) >= 0) {
        printf("❌ Fehler: Nachricht ist größer oder gleich n. Nutze kleinere Nachricht oder größere Primes.\n");
        return 1;
    }

    // Verschlüsseln
    mpz_powm(c, m, e, n);

    // Entschlüsseln
    mpz_powm(decrypted, c, d, n);

    // Zurück zu Text
    char decrypted_text[1024];
    mpz_to_string(decrypted_text, decrypted);

    // 🖨 Ausgabe
    printf("\n✅ RSA 2048-bit Demo\n");
    printf("-------------------------\n");
    printf("Ursprüngliche Nachricht: %s\n", plaintext);
    gmp_printf("Verschlüsselte Nachricht (Zahl):\n%Zd\n", c);
    printf("Entschlüsselte Nachricht: %s\n", decrypted_text);

    // Aufräumen
    mpz_clears(p, q, n, phi, e, d, m, c, decrypted, p1, q1, NULL);
    gmp_randclear(state);
    return 0;
}

