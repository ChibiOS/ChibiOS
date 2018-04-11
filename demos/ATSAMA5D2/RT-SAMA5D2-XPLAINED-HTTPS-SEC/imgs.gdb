#
# Switch to secure image symbols, without touching anything else
#
define si
    # Clear the symbol table
    symbol-file
    # Load the symbol table of the secure image
    symbol-file ../RT-SAMA5D2-XPLAINED-HTTPS-SEC/build/ch.elf
end

#
# Switch to non secure image symbols, without touching anything else
#
define nsi
    # Clear the symbol table
    symbol-file
    # Load the symbol table of the non secure image
    symbol-file ../RT-SAMA5D2-XPLAINED-HTTPS-NSEC/build/ch.elf
end


#
# Load non secure image into ddr
#
define lns
    # Load the non secure image
    set $oldpc=$pc
    load ../RT-SAMA5D2-XPLAINED-HTTPS-NSEC/build/ch.elf
    set $pc=$oldpc
end

#
# Set pc to the address pc+4
#
define spc
    set $pc=$pc+4
end
