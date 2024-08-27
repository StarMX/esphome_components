Example:
```yaml
templates:
  on_data_received:
    - logger.log:
        level: ERROR
        tag: "system"
        format: "%s data_received"
        args: [message.c_str()]
```
