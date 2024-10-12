# KFS

## ToDo

- [ ] Write png generator in python

## Version 1

### Signature (4 bytes): `0x00~0x03`

```0x0b 0x46 0x53 0x1A```

### Metadata 1 `0x04`

```
76543210
|||||||+- h-write 0 / v-write 1
||||||+-- dual mode (2bit colors)
|||||+
||||+
++++----- format version
```

### Metadata 2 `0x05`

Reserved

### Char size `0x06`

```
76543210
||||++++- char rows in 8bit units
++++----- char cols in 8bit units
```

### Padding `0x07~0x0d`

### Data Start Address `0x0e~0x0f`
