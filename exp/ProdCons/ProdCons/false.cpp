//int reader_counter = 0; //�����ڶ��߽��м���
//semaphore mutex = 1;// �Լ������Ļ������
//semaphore rw = 1;//��ʾ��ǰ�Ƿ��н����ڷ����ļ�
//
//
//reader() {
//	p(mutex);//ʹ�û����źű�֤�������ķ����ǻ����
//	if (reader_counter == 0) 
//		p(rw);
//	reader_counter++;
//	v(mutex);//�����������ͷŻ����ź���	
//	read();
//	p(mutex);
//	reader_counter--;//��������������������1
//	if (reader_counter == 0)
//		v(rw);
//	v(mutex);
//}
//
//
//writer() {
//	p(w);
//	write();
//	v(w);
//}