#include <unistd.h>
#include <stdio.h>
#include <string.h>

void change_color_led_status(void* value)
{
    char ch = *(char*)value;
	printf("ch: %c\n", ch);
	if(ch == '+')
	{
		printf("++++++++++++\n");
	}
	else if(ch == '-')
	{
		printf("------------\n");
	}
    else
	{
		printf("??????????????\n");
	}
}

int main()
{
    int ret=access("test",F_OK);
    if(ret==0)
    {
        printf("文件存在\n");
    }
    if(ret==-1)
    {
        printf("文件不存在\n");
    }
	
	printf("---------------------------------------------\n");
	if(access("test",F_OK))
    {
        printf("文件存在\n");
    }
    if(access("test",F_OK))
    {
        printf("文件不存在\n");
    }
	
	char buf[64] = {0};
	char *TTS = "你好，我是鸿雁电器的小燕。想听音乐吗";
	printf("TTS: %s\n", TTS);
	int len = snprintf(buf, 64, "{\"tts\":\"%s\"", TTS);
	buf[len] = '}';
	buf[len+1] = '\0';
	//write(voice_fd, buf, len);
	printf("通知语音 TTS : %s, len: %d\n", buf, len);
	
	char *TTS_str = "你好，我是鸿雁电器的小燕，想听音乐吗";
	len = strlen(TTS_str);
	int delayTime = len / 9 + 1;
	printf("通知语音 len: %d, delayTime: %d\n", len, delayTime);
	
	char * str_test = "-";
	len = strlen(str_test);
	printf("通知语音 str_test : %s, len: %d\n", str_test, len);
	char val = str_test[0];
	
	change_color_led_status(&val);
	
	int max = 10, c = 5;
	max = c > max ? c:max;
	printf("max: %d\n", max);
	
    return 0;
}