
# Unsupported Transaction Types

The examples found here illustrate how these transaction types could be implemented,  
as well as why they are not supported.

These are **not** final, tested, or guaranteed as working;  
however, this should help make implementation easier when needed--if ever.

## What's currently here

* Type 2: Delegate Registration
  >Not Supported.
  >
  >Delegates Forge; forging cannot be done using a Ledger.
* Type 6: MultiPayment
  >Not supported
  >
  >Max MultiPayment just won't fit.  
  >This type can be upwards of ~2MB;  
  >Ledger NanoS only has 4KiB(~4KB) of runtime memory.
  >
  >It's also unreasonable to expect users to consistently validate hundreds-of-payments;  
  >especially on a device the size of a pack of chewing gum.  
  >This could encourage users to haphazardly skip over verifying payments.
  >
  >Additionally, using checksum-based validation exposes users to potential attacks from compromised wallets.
  >
  >Considering the above,  
  >it's better to protect users and err on the side of security by not supporting MultiPayment at this time.
* Type 7: Delegate Resignation
  >Not Supported.
  >
  >Delegates Forge; forging cannot be done using a Ledger.
