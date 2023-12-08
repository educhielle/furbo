# FURBO

Furbo is a framework that improves performance of FHE libraries through optmized ciphertext management and caching techniques.

## Dependencies

SEAL library. To set it up locally within Furbo repository, run:
```
cd 3p
sh seal_unx.sh
```

## Applications

### FLAGS:

TEMPLATE:
```
TEMPLATE=0 // compile without Furbo
TEMPLATE=8 // compile with state-of-the-art Furbo
```
`TEMPLATE=1` to `TEMPLATE=7` are deprecated.

Size limit of OOM table:
```
SIZE=-1 // no size limit
SIZE=0 // no OOM, only ACRM
SIZE=S // the size limit is S, where S is an integer > 0
```

Operations that OOM will record (0: no, 1: yes). The default configuration is shown below:
```
CT_ADD=0
CT_MUL=0
CT_SUB=0
PT_ADD=0
PT_MUL=1
PT_SUB=0
```

### Matrix Multiplication

Compile without Furbo:
`make compile TEMPLATE=0`

Compile with Furbo ACRM-only:
`make compile TEMPLATE=8 SIZE=0`

Compile with Furbo ACRM + OOM:
`make compile TEMPLATE=8 SIZE=-1 CT`

### Convolutional Layer

### CryptoNets



## Cite us

To appear on DATE 2024.