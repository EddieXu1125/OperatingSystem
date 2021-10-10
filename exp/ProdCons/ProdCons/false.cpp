//int reader_counter = 0; //对现在读者进行计数
//semaphore mutex = 1;// 对计数器的互斥访问
//semaphore rw = 1;//表示当前是否有进程在访问文件
//
//
//reader() {
//	p(mutex);//使用互斥信号保证计数器的访问是互斥的
//	if (reader_counter == 0) 
//		p(rw);
//	reader_counter++;
//	v(mutex);//计数结束，释放互斥信号量	
//	read();
//	p(mutex);
//	reader_counter--;//读操作结束，计数器减1
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