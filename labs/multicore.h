#pragma once
#include <atomic>
//
// INVARIANT: w_deleted_count <= w_deleting_count <= w_cached_read_count <= shared_read_count <= r_reading_count <= r_cached_write_count <= shared_write_count <= w_writing_count <= w_deleted_count + MAX_SIZE
//
// INVARIANT:      w_writing_count      - w_deleted_count     <= MAX_SIZE
// =========>      w_writing_count      - w_cached_read_count <= MAX_SIZE
// =========>      shared_write_count   - w_cached_read_count <= MAX_SIZE
// =========>      shared_write_count   - shared_read_count   <= MAX_SIZE
//
//
// INVARIANT: 0 <= r_cached_write_count - r_reading_count
// =========> 0 <= r_cached_write_count - shared_read_count
// =========> 0 <= shared_write_count   - shared_read_count
//
//
// THEOREM: =========> 0 <= shared_write_count   - shared_read_count   <= MAX_SIZE
//





//
//
// Channel/Queue:
//
// Shared between Producer and Consumer
//
struct channel_t{
  public:

      std:: atomic<int> read_head,write_head;

  public:

    //
    // Intialize
    //
    channel_t(){

      read_head=0;
      write_head=0;


    }
};


//
// Producer's (not shared)
//
struct writeport_t{
public:
    //insert your code here

      int read_head,deleted_head,write_head,deleting_head;
      size_t length;
public:

  //
  // Intialize
  //
  writeport_t(size_t tsize)
  {
      read_head=0;
      deleted_head=(int)tsize-1;
      deleting_head=(int)tsize-1;
      write_head=0;
      length=tsize;
  }

public:

  //
  // no of entries available to write
  //
  // helper function for write_canreserve
  //
  size_t write_reservesize(){

    // insert your code here

    if(write_head==deleted_head)
      return 1;
    else if(write_head>deleted_head)
      return length+(size_t)(-write_head+deleted_head+1);
    else
      return (size_t)(deleted_head-write_head+1);
  }

  //
  // Can write 'n' entries?
  //
  bool write_canreserve(size_t n){

    // insert your code here

    return n<=write_reservesize();
  }

  //
  // Reserve 'n' entries for write
  //
  size_t write_reserve(size_t n){
    write_head=(size_t)(write_head+(int)n)%length;

    return write_head;
  }

  //
  // Commit
  //
  // Read/Write shared memory data structure
  //
  void write_release(channel_t& ch){
      // insert your code here
    ch.write_head=write_head;
  }




public:

  //
  //
  // Read/Write shared memory data structure
  //
  void read_acquire(channel_t& ch){

    read_head=ch.read_head;

  }




  //
  // No of entires available to delete
  //
  size_t delete_reservesize(){
    //insert your code here
    
    if(read_head<deleting_head)
      return length+(size_t)(read_head-deleting_head-1);
    else
      return (size_t)(-deleting_head+read_head-1);
    return 0;
  }

  //
  // Can delete 'n' entires?
  //
  bool delete_canreserve(size_t n){
    //insert your code here

    return n<=delete_reservesize();
  }

  //
  // Reserve 'n' entires for deletion
  //
  size_t delete_reserve(size_t n){
    //insert your code here
    deleting_head=(deleting_head+n)%length;
    return deleting_head;
  }


  //
  // Update the state, if any.
  //
  void delete_release(){
    //insert your code here
      deleted_head=deleting_head;
      return ;
  }


};


//
// Consumer's (not shared)
//
//
struct readport_t{
public:

  //insert your code here

      int read_head,write_head;
      size_t length;

public:
  //
  // Initialize
  //
  readport_t(size_t tsize)
  {

      //insert your code here
      read_head=0;
      write_head=0;
      length=tsize;
  }
  public:

  //
  // Read/Write shared memory data structure
  //
  void write_acquire(channel_t& ch){

   write_head=ch.write_head;
  }

  //
  // no of entries available to read
  //
  size_t read_reservesize(){

     //insert your code here
    return (length+write_head-read_head)%length;
    
  }

  //
  // Can Read 'n' entires?
  //
  bool read_canreserve(size_t n){

    //insert your code here

    return n<=read_reservesize();
  }

  //
  // Reserve 'n' entires to be read
  //
  size_t read_reserve(size_t n){

    //insert your code here
    read_head=(read_head+(int)n)%length;
    return (size_t)read_head;
  }

  //
  // Read/write shared memory data structure
  //
  void read_release(channel_t& ch){

    //insert your code here
    ch.read_head=read_head;

  }

};


