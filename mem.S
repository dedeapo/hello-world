.text
.global mem_init
mem_init:
    
    ldr r0, =0x7e00f120
    mov r1, #0x8
    str r1, [r0]

    ldr r0, =0x7e001004  @�ڴ��������Ĵ���
    
    mov r1, #0x4         @�����ֲ��֪��Ҫ�Ƚ�������ģʽ
    str r1, [r0]

    ldr r0, =0x7e001010  @ˢ�¼Ĵ�����ַ
    ldr r1, =( ( 7800 / ( 1000000000/133000000 ) + 1 ) )      @����ˢ��ʱ��
    str r1, [r0]

    ldr r0, =0x7e001014  @CAS latency�Ĵ���
    mov r1, #(3 << 1)
    str r1, [r0]

    ldr r0, =0x7e001018  @t_DQSS�Ĵ���
    mov r1, #0x1
    str r1, [r0]

    ldr r0, =0x7e00101c  @T_MRD�Ĵ���
    mov r1, #0x2
    str r1, [r0]

    ldr r0, =0x7e001020   @t_RAS�Ĵ���
    ldr r1, =( ( 45 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001024   @t_RC�Ĵ���
    ldr r1, =( ( 68 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001028   @t_RCD�Ĵ���
    ldr r1, =( ( 23 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e00102c   @t_RFC�Ĵ���
    ldr r1, =( ( 80 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001030   @t_RP�Ĵ���
    ldr r1, =( ( 23 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001034   @t_rrd�Ĵ���
    ldr r1, =( ( 15 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001038   @t_wr�Ĵ���
    ldr r1, =( ( 15 / ( 1000000000 / 133000000 ) + 1 ) )
 @  ldr r2, [r0]
    str r1, [r0]

    ldr r0, =0x7e00103c   @t_wtr�Ĵ���
    mov r1, #0x07
    str r1, [r0]

    ldr r0, =0x7e001040   @t_xp�Ĵ���
    mov r1, #0x02
    str r1, [r0]

    ldr r0, =0x7e001044   @t_xsr�Ĵ���
    ldr r1, =( ( 120 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e001048   @t_esr�Ĵ���
    ldr r1, =( ( 120 / ( 1000000000 / 133000000 ) + 1 ) )
    str r1, [r0]

    ldr r0, =0x7e00100c   @�ڴ�������üĴ���
    ldr r1, =0x00010012   @���ÿ�����
    str r1, [r0]

    ldr r0, =0x7e00104c   @32λDRAM���ÿ��ƼĴ���
    ldr r1, =0x0b45
    str r1, [r0]

    ldr r0, =0x7e001200   @Ƭѡ�Ĵ���
    ldr r1, =0x150f8
    str r1, [r0]

    ldr r0, =0x7e001304   @�û����üĴ���
    mov r1, #0x0
    str r1, [r0]

    ldr r0, =0x7e001008
    ldr r1, =0x000c0000
    str r1, [r0]

    ldr r1, =0x00000000
    str r1, [r0]

    ldr r1, =0x00040000
    str r1, [r0]

    ldr r1, =0x000a0000
    str r1, [r0]

    ldr r1, =0x00080032
    str r1, [r0]

    ldr r0, =0x7e001004
    mov r1, #0x0
    str r1, [r0]

check_dmc1_ready:
    
    ldr r0, =0x7e001000 
    ldr r1, [r0]
    mov r2, #0x3
    and r1, r1, r2
    cmp r1, #0x1
    bne check_dmc1_ready
    nop
    mov pc, lr
