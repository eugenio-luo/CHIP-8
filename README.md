# CHIP-8 virtual machine

It's a virtual machine for programs written in the CHIP-8 interpreted language

## How to compile and run

### requirements

- `SDL2`

To compile it:

```bash
make
```
To compile with tests:

```bash
make test
```

To run it:

```bash
./CHIP-8.out [program]
```
`program` should be a CHIP-8 program, if it is the test version of the program, `program` is ignored and the program won't be run.