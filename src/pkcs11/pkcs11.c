#include <stdio.h>
#include "pkcs11.h"

void aes_ecb_encrypt(CK_FUNCTION_LIST_3_0_PTR pkcs11, CK_SESSION_HANDLE session, CK_OBJECT_HANDLE key, unsigned char *plaintext, unsigned char *ciphertext, size_t length) {
    CK_RV rv;
    CK_MECHANISM mechanism = { CKM_AES_ECB, NULL, 0 };
    
    rv = pkcs11->CK_C_EncryptInit(session, &mechanism, key);
    if (rv != CKR_OK) {
        printf("C_EncryptInit failed: %lu\n", rv);
        return;
    }

    CK_ULONG cipherLength = length;
    rv = pkcs11->C_Encrypt(session, plaintext, length, ciphertext, &cipherLength);
    if (rv != CKR_OK) {
        printf("C_Encrypt failed: %lu\n", rv);
        return;
    }
    
    printf("Ciphertext length: %lu\n", cipherLength);
}

int main() {
    CK_RV rv;
    CK_C_INITIALIZE_ARGS initArgs = {  };  // Initialize with appropriate values

    // Initialize the PKCS#11 module
    rv = CK_C_Initialize(&initArgs);
    if (rv != CKR_OK) {
        printf("C_Initialize failed: %lu\n", rv);
        return 1;
    }

    // Open session and perform other necessary steps (login, key retrieval, etc.)

    // Example usage
    unsigned char plaintext[16] = { /* ... */ };
    unsigned char ciphertext[16];
    
    aes_ecb_encrypt(pkcs11, session, key, plaintext, ciphertext, sizeof(plaintext));

    // Clean up and finalize
    // ...

    return 0;
}