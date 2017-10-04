# Magnify-light

## A magnifier to investigate PMT data.

### Usage

```
cd scripts/
root -l loadClasses.C 'Magnify.C("path/to/rootfile")'
```

The call to ROOT can be be called somewhat more easily via a shell
script wrapper.  It assumes to stay in the source directory:

```
/path/to/magnify/magnify.sh /path/to/rootfile.root
```

### Example files

An example ROOT file can be found at http://www.phy.bnl.gov/xqian/talks/wire-cell/flashes/flash_11110_135_6780.root

