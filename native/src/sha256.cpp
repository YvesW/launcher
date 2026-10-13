#include "packr.h"

#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
#else
#include <CommonCrypto/CommonDigest.h>
#endif

bool sha256File(const char* path, std::string& result) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }

    unsigned char digest[32];
    char buffer[65536];
    bool success = false;
#ifdef _WIN32
    BCRYPT_ALG_HANDLE algorithm = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    if (BCryptOpenAlgorithmProvider(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0) < 0) {
        return false;
    }
    if (BCryptCreateHash(algorithm, &hash, nullptr, 0, nullptr, 0, 0) < 0) {
        BCryptCloseAlgorithmProvider(algorithm, 0);
        return false;
    }
#else
    CC_SHA256_CTX hash;
    if (!CC_SHA256_Init(&hash)) {
        return false;
    }
#endif

    do {
        file.read(buffer, sizeof(buffer));
        const auto count = file.gcount();
        if (count > 0) {
#ifdef _WIN32
            if (BCryptHashData(hash, reinterpret_cast<PUCHAR>(buffer), static_cast<ULONG>(count), 0) < 0) {
                break;
            }
#else
            if (!CC_SHA256_Update(&hash, buffer, static_cast<CC_LONG>(count))) {
                break;
            }
#endif
        }
        if (file.bad() || (file.fail() && !file.eof())) {
            break;
        }
        if (file.eof()) {
#ifdef _WIN32
            success = BCryptFinishHash(hash, digest, sizeof(digest), 0) >= 0;
#else
            success = CC_SHA256_Final(digest, &hash) != 0;
#endif
            break;
        }
    } while (true);

#ifdef _WIN32
    BCryptDestroyHash(hash);
    BCryptCloseAlgorithmProvider(algorithm, 0);
#endif
    if (!success) {
        return false;
    }

    static const char hex[] = "0123456789abcdef";
    result.clear();
    for (unsigned char byte : digest) {
        result += hex[byte >> 4];
        result += hex[byte & 15];
    }
    return true;
}
