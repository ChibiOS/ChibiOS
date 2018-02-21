
call %OPENSSL%\openssl dgst -sha1 -c -binary -out sha_sha1_3 plaintext_2
ren  sha_sha1_3 sha_sha1_3.enc

call %OPENSSL%\openssl dgst -sha1 -c -binary -out sha_sha1_56 plaintext_3
ren  sha_sha1_56 sha_sha1_56.enc

call %OPENSSL%\openssl dgst -sha256 -c -binary -out sha_sha256_3 plaintext_2
ren  sha_sha256_3 sha_sha256_3.enc

call %OPENSSL%\openssl dgst -sha256 -c -binary -out sha_sha256_56 plaintext_3
ren  sha_sha256_56 sha_sha256_56.enc


call %OPENSSL%\openssl dgst -sha512 -c -binary -out sha_sha512_3 plaintext_2
ren  sha_sha512_3 sha_sha512_3.enc


call %OPENSSL%\openssl dgst -sha512 -c -binary -out sha_sha512_56 plaintext_3
ren  sha_sha512_56 sha_sha512_56.enc