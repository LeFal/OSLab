#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/bio.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/bio.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>


unsigned char* SIP;
unsigned char* DIP; 

// Tool: Integer IP to Char Array (for print)
void convert_to_ip(char* global ,int ip)
{
    static unsigned char bytes[4];
    global[0] = ip & 0xFF;
    global[1] = (ip >> 8) & 0xFF;
    global[2] = (ip >> 16) & 0xFF;
    global[3] = (ip >> 24) & 0xFF; 
	return;
}

static unsigned int my_hook_fn_pre_routing(void *priv,
							   struct sk_buff *skb,
							   const struct nf_hook_state *state
								){
	/* 후킹함수 작성 */
	struct iphdr *ih = ip_hdr(skb);
	struct tcphdr *th = tcp_hdr(skb);

	unsigned char protocol = ih->protocol; // protocol
	unsigned short Sport = ntohs(th->source); // Sport
	unsigned short Dport = ntohs(th->dest); // Dport
	convert_to_ip(SIP,ih->saddr); // Source IP
	convert_to_ip(DIP,ih->daddr); // Destination IP


	printk("PRE_ROUTING packet| protocol: %d, Sport: %hu, Dport: %hu, SIP: %d.%d.%d.%d, DIP: %d.%d.%d.%d\n",
   		protocol, Sport, Dport,SIP[0],SIP[1],SIP[2],SIP[3],DIP[0],DIP[1],DIP[2],DIP[3]);
    //서버의 33333 포트에서 온 패킷을 Forwarding 대상으로 지정
    if (Sport == (unsigned short)33333) {

    	// change Port to 7777
    	// 문제상황 htons, ntohs의 사용법을 몰라 결과가 정상적으로 나오지 않았음. 
    	th->source = htons((unsigned short)7777);
        th->dest = htons((unsigned short)7777);
		ih->daddr = htonl((unsigned long)3232261120); //192.168.1.0
    
        // Packet Forwarding (Change destination of packet)
		printk("%d\n",ih->daddr);
		printk("%d\n",ih->saddr);

        convert_to_ip(SIP,ih->saddr); // Source IP
        convert_to_ip(DIP,ih->daddr); // Destination IP

        printk("CHANGED SIP: %d.%d.%d.%d\n", SIP2[0],SIP2[1],SIP2[2],SIP2[3]);
        printk("CHANGED DIP: %d.%d.%d.%d\n", DIP2[0],DIP2[1],DIP2[2],DIP2[3]);

	printk("%d\n",ih->daddr);
        printk("%d\n",ih->saddr);

	printk("CHANGED PRE_ROUTING packet| protocol: %d, Sport: %hu, Dport: %hu, SIP: %d.%d.%d.%d, DIP: %d.%d.%d.%d\n",
protocol, Sport, Dport,SIP2[0],SIP2[1],SIP2[2],SIP2[3],DIP2[0],DIP2[1],DIP2[2],DIP2[3]);


	return NF_ACCEPT;
    }

    return NF_DROP;
}

static unsigned int my_hook_fn_forward(void *priv,
							   struct sk_buff *skb,
							   const struct nf_hook_state *state
								){
	/* 후킹함수 작성 */
	struct iphdr *ih = ip_hdr(skb);
	struct tcphdr *th = tcp_hdr(skb);

	unsigned char protocol = ih->protocol; // protocol
	unsigned short Sport = ntohs(th->source); // Sport
	unsigned short Dport = ntohs(th->dest); // Dport
	convert_to_ip(SIP,ih->saddr); // Source IP
	convert_to_ip(DIP,ih->daddr); // Destination IP

    printk("FORWARD packet| protocol: %d, Sport: %hu, Dport: %hu, SIP: %d.%d.%d.%d, DIP: %d.%d.%d.%d\n",
    	protocol, Sport, Dport,SIP[0],SIP[1],SIP[2],SIP[3],DIP[0],DIP[1],DIP[2],DIP[3]);

    return NF_ACCEPT;
}

// 
static unsigned int my_hook_fn_post_routing(void *priv,
							   struct sk_buff *skb,
							   const struct nf_hook_state *state
								){
	struct iphdr *ih = ip_hdr(skb);
	struct tcphdr *th = tcp_hdr(skb);

	unsigned char protocol = ih->protocol; // protocol
	unsigned short Sport = ntohs(th->source); // Sport
	unsigned short Dport = ntohs(th->dest); // Dport
	convert_to_ip(SIP,ih->saddr); // Source IP
	convert_to_ip(DIP,ih->daddr); // Destination IP

    printk("POST_ROUTING packet| protocol: %d, Sport: %hu, Dport: %hu, SIP: %d.%d.%d.%d, DIP: %d.%d.%d.%d\n",
    	protocol, Sport, Dport,SIP[0],SIP[1],SIP[2],SIP[3],DIP[0],DIP[1],DIP[2],DIP[3]);

    return NF_ACCEPT;
}




// 문제 상황 : hook operation 구조체는 하나만 있어야 한다고 생각해서 어떻게 3개를 넣을지 몰라서 약간 고생했음
static struct nf_hook_ops my_nf_ops_pre_routing = {
	.hook = my_hook_fn_pre_routing,
	.pf = PF_INET,
	.hooknum = NF_INET_PRE_ROUTING,
	.priority = NF_IP_PRI_FIRST
};

static struct nf_hook_ops my_nf_ops_forward = {
	.hook = my_hook_fn_forward,
	.pf = PF_INET,
	.hooknum = NF_INET_FORWARD,
	.priority = NF_IP_PRI_FIRST
};

static  struct nf_hook_ops my_nf_ops_post_routing = {
	.hook = my_hook_fn_post_routing,
	.pf = PF_INET,
	.hooknum = NF_INET_POST_ROUTING,
	.priority = NF_IP_PRI_FIRST
};


// run when init
// 문제 : nf_register_hook 파라미터로 struct 을 집어넣지 않고, 함수명을 집어 넣어 디버깅하는데 고생함
static int __init my_init(void)
{
	nf_register_hook(&my_nf_ops_pre_routing);
	nf_register_hook(&my_nf_ops_forward);
	nf_register_hook(&my_nf_ops_post_routing);
	return 0;
}


static void __exit my_exit(void)
{
	printk(KERN_INFO "exit\n");
	nf_unregister_hook(&my_nf_ops_pre_routing);
	nf_unregister_hook(&my_nf_ops_forward);
	nf_unregister_hook(&my_nf_ops_post_routing);
	return;
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("OHLEE");
MODULE_DESCRIPTION("f*ck this shit");
MODULE_LICENSE("GPL");
MODULE_VERSION("NEW");
