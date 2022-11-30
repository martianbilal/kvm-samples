#include <stdio.h>
#include <linux/kvm.h>
#include <assert.h>

#define KVM_IOCTL_NR 51

#define STRINGIFY(x) #x

int kvm_ids[KVM_IOCTL_NR] = {
    KVM_RUN,
    KVM_GET_API_VERSION,
    KVM_CREATE_VM,
    KVM_ENABLE_CAP,
    KVM_GET_MSR_INDEX_LIST,
    KVM_GET_MSR_FEATURE_INDEX_LIST,
    KVM_SET_IDENTITY_MAP_ADDR,
    KVM_SET_TSS_ADDR,
    KVM_CREATE_IRQCHIP,
    KVM_IOEVENTFD,
    KVM_GET_SUPPORTED_CPUID,
    KVM_X86_GET_MCE_CAP_SUPPORTED,
    KVM_GET_MSRS,
    KVM_CREATE_VCPU,
    KVM_GET_VCPU_MMAP_SIZE,
    KVM_GET_TSC_KHZ,
    KVM_X86_SETUP_MCE,
    KVM_SET_CPUID2,
    KVM_SET_MSRS,
    KVM_GET_CLOCK,
    KVM_SET_USER_MEMORY_REGION,
    KVM_SET_GSI_ROUTING,
    KVM_REGISTER_COALESCED_MMIO,
    KVM_UNREGISTER_COALESCED_MMIO,
    KVM_CREATE_PIT2,
    KVM_SET_SREGS,
    KVM_SET_TSC_KHZ,
    KVM_SET_REGS,
    KVM_SET_XSAVE,
    KVM_SET_XCRS,
    KVM_SET_VCPU_EVENTS,
    KVM_SET_MP_STATE,
    KVM_SET_DEBUGREGS,
    KVM_GET_VCPU_EVENTS,
    KVM_GET_MP_STATE,
    KVM_GET_REGS,
    KVM_GET_XSAVE,
    KVM_GET_XCRS,
    KVM_GET_SREGS,
    KVM_GET_LAPIC,
    KVM_GET_DEBUGREGS,
    KVM_GET_PIT2,
    KVM_SET_PIT2,
    KVM_SET_IRQCHIP,
    KVM_TPR_ACCESS_REPORTING,
    KVM_SET_VAPIC_ADDR,
    KVM_SET_LAPIC,
    KVM_SET_CLOCK,
    KVM_KVMCLOCK_CTRL,
    KVM_SMI,
    KVM_GET_DIRTY_LOG
};

char *kvm_ids_strings[KVM_IOCTL_NR] = {
    "KVM_RUN",
    "KVM_GET_API_VERSION",
    "KVM_CREATE_VM",
    "KVM_ENABLE_CAP",
    "KVM_GET_MSR_INDEX_LIST",
    "KVM_GET_MSR_FEATURE_INDEX_LIST",
    "KVM_SET_IDENTITY_MAP_ADDR",
    "KVM_SET_TSS_ADDR",
    "KVM_CREATE_IRQCHIP",
    "KVM_IOEVENTFD",
    "KVM_GET_SUPPORTED_CPUID",
    "KVM_X86_GET_MCE_CAP_SUPPORTED",
    "KVM_GET_MSRS",
    "KVM_CREATE_VCPU",
    "KVM_GET_VCPU_MMAP_SIZE",
    "KVM_GET_TSC_KHZ",
    "KVM_X86_SETUP_MCE",
    "KVM_SET_CPUID2",
    "KVM_SET_MSRS",
    "KVM_GET_CLOCK",
    "KVM_SET_USER_MEMORY_REGION",
    "KVM_SET_GSI_ROUTING",
    "KVM_REGISTER_COALESCED_MMIO",
    "KVM_UNREGISTER_COALESCED_MMIO",
    "KVM_CREATE_PIT2",
    "KVM_SET_SREGS",
    "KVM_SET_TSC_KHZ",
    "KVM_SET_REGS",
    "KVM_SET_XSAVE",
    "KVM_SET_XCRS",
    "KVM_SET_VCPU_EVENTS",
    "KVM_SET_MP_STATE",
    "KVM_SET_DEBUGREGS",
    "KVM_GET_VCPU_EVENTS",
    "KVM_GET_MP_STATE",
    "KVM_GET_REGS",
    "KVM_GET_XSAVE",
    "KVM_GET_XCRS",
    "KVM_GET_SREGS",
    "KVM_GET_LAPIC",
    "KVM_GET_DEBUGREGS",
    "KVM_GET_PIT2",
    "KVM_SET_PIT2",
    "KVM_SET_IRQCHIP",
    "KVM_TPR_ACCESS_REPORTING",
    "KVM_SET_VAPIC_ADDR",
    "KVM_SET_LAPIC",
    "KVM_SET_CLOCK",
    "KVM_KVMCLOCK_CTRL",
    "KVM_SMI",
    "KVM_GET_DIRTY_LOG"
};

int main(int argc, char *argv[]) {

    FILE *fptr;

    if(argc <= 1 || argc >= 3) {
        printf("./print-ioctl-ids filename\n");
        return 1; 
    }

    fptr = fopen(argv[1], "w");
    assert(fptr);
    

    fprintf(fptr, "{\n");
    for(int i = 0; i < KVM_IOCTL_NR; i++){
        // fprintf(fptr, "%s,%p\n", kvm_ids_strings[i], kvm_ids[i]);
        fprintf(fptr, "\t\'%s\' : %p,\n", kvm_ids_strings[i], kvm_ids[i]);
    }
    fprintf(fptr, "}\n");

    fclose(fptr);
    
    return 0;
}