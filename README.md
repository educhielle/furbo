# FURBO

Furbo is a plug-and-play framework designed to optimize ciphertext management for faster Fully Homomorphic Encryption (FHE) computation. This framework acts as middleware between any FHE compiler and FHE library, employing smart ciphertext memory management and caching techniques to enhance performance without modifying underlying code.

## Table of Contents

- [Paper Information](#paper-information)
- [Dependencies](#dependencies)
- [Usage](#usage)
  - [Compiling](#compiling)
  - [Fully Connected Layer](#fully-connected-layer)
  - [Convolutional Layer](#convolutional-layer)
  - [CryptoNets CNN](#cryptonets-cnn)
- [License](#license)

## Paper Information

- **Title:** Optimizing Ciphertext Management for Faster Fully Homomorphic Encryption Computation
- **Conference:** Design, Automation and Test in Europe Conference (DATE) 2024

> **Abstract:**
> Fully Homomorphic Encryption (FHE) is the pinnacle of privacy-preserving outsourced computation as it enables meaningful computation to be performed in the encrypted domain without the need for decryption or back-and-forth communication between the client and service provider. Nevertheless, FHE is still orders of magnitude slower than unencrypted computation, which hinders its widespread adoption. In this work, we propose \name, a plug-and-play framework that can act as middleware between any FHE compiler and any FHE library. Our proposal employs smart ciphertext memory management and caching techniques to reduce data movement and computation, and can be applied to FHE applications without modifications to the underlying code. Experimental results using Microsoft SEAL as the base FHE library and focusing on privacy-preserving Machine Learning as a Service show up to 2x performance improvement in the fully-connected layers, and up to 24x improvement in the convolutional layers without any code change.

### Cite us

E. Chielle, O. Mazonka, and M. Maniatakos, "Optimizing Ciphertext Management for Faster Fully Homomorphic Encryption Computation," Design, Automation and Test in Europe Conference (DATE), 2024.

## Dependencies

Furbo has minimal dependencies. Ensure you have a C++ compiler and install the Microsoft SEAL library.
To setup SEAL locally within Furbo repository, run:
```
cd 3p
sh seal_unx.sh
```

## Usage

### Compiling:

#### Flags:

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

#### Examples:

Compile without Furbo:
`make compile TEMPLATE=0`

Compile with Furbo ACRM-only:
`make compile TEMPLATE=8 SIZE=0`

Compile with Furbo ACRM + OOM:
`make compile TEMPLATE=8 SIZE=-1 CT_ADD=0 CT_MUL=0 CT_SUB=0 PT_ADD=0 PT_MUL=1 PT_SUB=0`

### Fully-Connected Layer

### Convolutional Layer

### CryptoNets CNN

### License
