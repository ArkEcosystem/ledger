Parsing serialised transaction:
```
  var buf = new Buffer(hexString, "hex");
  tx.type = buf.readInt8(0);
  tx.timestamp = buf.readUInt32LE(1);
  tx.senderPublicKey = hexString.substring(10,10+33*2);
  tx.amount = buf.readUInt32LE(38+21+64);
  tx.fee = buf.readUInt32LE(38+21+64+8);
  tx.vendorFieldHex = hexString.substring(76+42,76+42+128);
  tx.recipientId = bs58check.encode(buf.slice(38,38+21));
 ```
