; ---------------------------------------------------------------------------
; vectors.s
; ---------------------------------------------------------------------------
;
; Defines the interrupt vector table.

.import    _init
.import    _nmi_int_set_frameflag, _irq_int_check_render_queue

.segment  "VECTORS"

.addr      _nmi_int_set_frameflag    ; NMI vector
.addr      _init       ; Reset vector
.addr      _irq_int_check_render_queue    ; IRQ/BRK vector