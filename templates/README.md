Example:
```yaml
templates:
  on_data_received:
    - logger.log:
        level: ERROR
        tag: "system"
        format: "%s data_received"
        args: [message.c_str()]

text:
  - platform: template
    name: Send
    icon: "mdi:cursor-text"
    mode: text
    set_action: 
      - templates.send:
          message: !lambda return x.c_str();
```
