# HMAC Service

## Требования к окружению и установка

1. Установить OpenSSL.
2. Установить cpprestsdk.
3. Установить компилятор GCC или Clang.
4. Запустить скрипт сборки build.sh из корня проекта.

## Как сгенерировать секрет (пример команд для выбранного стека)
TODO

## Формат config.json
```
{
    "hmac_alg": "SHA256",
    "secret": "<base64>",
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

