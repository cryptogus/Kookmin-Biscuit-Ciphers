#include "rsa2048.h"

int main() {
    RSA *private_key = NULL;
    RSA *public_key = NULL;

    if (!generate_key_pair(&private_key, &public_key)) {
        fprintf(stderr, "Failed to generate RSA key pair.\n");
        return 1;
    }

    const char *original_text = "OpenSSL RSA2048~~~~~~~~~~~!!!!!";
    size_t text_length = strlen(original_text);

    unsigned char cipher_text[KEY_LENGTH / 8];
    char decrypted_text[KEY_LENGTH / 8];

    // 암호화
    int encrypted_length = rsa_encrypt(original_text, text_length, public_key, cipher_text);
    if (encrypted_length == -1) {
        fprintf(stderr, "Encryption failed.\n");
        return 1;
    }

    // 복호화
    int decrypted_length = rsa_decrypt(cipher_text, encrypted_length, private_key, decrypted_text);
    if (decrypted_length == -1) {
        fprintf(stderr, "Decryption failed.\n");
        return 1;
    }

    printf("Original Text: %s\n\n", original_text);
    printf("Encrypted Text: ");
    for (int i = 0; i < encrypted_length; ++i) {
        printf("%02x", cipher_text[i]);
    }
    printf("\n\n");
    printf("Decrypted Text: %s\n", decrypted_text);

    // 메모리 해제
    RSA_free(private_key);
    RSA_free(public_key);

    return 0;
}