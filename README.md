# HMAC Service

## Требования к окружению и установка

1. Установить OpenSSL: sudo apt-get install libssl-dev.
2. Установить cpprestsdk: sudo apt-get install libcpprest-dev.
3. Установить nlohmann/json: sudo apt-get install nlohmann-json3-dev.
4. Установить компилятор GCC версии 13, или старше.
5. Запустить скрипт сборки build.sh из корня проекта.

## Как сгенерировать секретЫ
TODO

## Формат config.json
```
{
    "hmac_alg": "SHA256",
    "secret": "<base64url>",
    "log_level": "info/warn/error",
    "listen": "0.0.0.0:8080",
    "max_msg_size_bytes": 1048576
}
```

## Запуск сервера
Запустить скрипт run-server.sh из корня проекта.

## Примеры curl
### Подписать сообщение:
```
curl -sS -X POST http://localhost:8080/sign \
  -H 'Content-Type: application/json' \
  -d '{"msg":"hello"}'
```

###  Проверка валидности электронной подписи:
```
curl -sS -X POST http://localhost:8080/verify \
  -H 'Content-Type: application/json' \
  -d '{"msg":"hello","signature":"<скопировать из /sign>"}'
```

## Ограничения учебной реализации
1. HMAC ≠ асимметричная ЭП;
2. нет многоключевой валидации;
3. ротация простая.

