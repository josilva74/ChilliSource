/*
 *  FileStream.h
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MOFLO_CORE_FILEIO_FILE_STREAM_
#define _MOFLO_CORE_FILEIO_FILE_STREAM_

#include <string>
#include <cstdio>
#include <fstream>

#include <ChilliSource/Core/Cryptographic/SHA1.h>

namespace moFlo
{
	namespace Core
	{
		//=================================================================
		/// File Mode
		///
		/// FileMode is used to specify how the file is going to be opened and read.
		//=================================================================
		enum FILE_MODE
		{
			FM_READ,
			FM_READ_BINARY,
			FM_WRITE,
			FM_WRITE_APPEND,
			FM_WRITE_AT_END,
			FM_WRITE_TRUNCATE,
			FM_WRITE_BINARY,
			FM_WRITE_BINARY_APPEND,
			FM_WRITE_BINARY_AT_END,
			FM_WRITE_BINARY_TRUNCATE
		};
		//=================================================================
		/// Seek Dir
		///
		/// An enum describing from where a seek should occur.
		//=================================================================
		enum SEEK_DIR
		{
			SD_BEGINNING,
			SD_CURRENT,
			SD_END
		};
		//=================================================================
		/// IFileStream
		///
		/// A filestream is used for all reading and writing of files.
		/// This will emulate the functionality of fstream and allows for
		/// cross platform file reading.
		//=================================================================
		class IFileStream
		{
		public:
			//--------------------------------------------------------------------------------------------------
			/// Destructor
			//--------------------------------------------------------------------------------------------------
            virtual ~IFileStream(){}
			//--------------------------------------------------------------------------------------------------
			/// Is Open
			///
			/// @return whether or not the stream is open
			//--------------------------------------------------------------------------------------------------
			virtual bool IsOpen();
			//--------------------------------------------------------------------------------------------------
			/// Is Bad
			///
			/// @return Checks that the file stream is not corrupt. this calls both fstream::fail() and
			///			fstream::bad()
			//--------------------------------------------------------------------------------------------------
			virtual bool IsBad();
			//--------------------------------------------------------------------------------------------------
			/// End Of File
			///
			/// @return Checks whether the end of the file has been reached.
			//--------------------------------------------------------------------------------------------------
			virtual bool EndOfFile();
			//--------------------------------------------------------------------------------------------------
			/// Close
			///
			/// Closes the filestream.
			//--------------------------------------------------------------------------------------------------
			virtual void Close();
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// @return the next character in the file stream as an integer
			//--------------------------------------------------------------------------------------------------
			virtual s32 Get();
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// Reads the next character into outbyChar
			///
			/// @param Container for the output value.
			//--------------------------------------------------------------------------------------------------
			virtual void Get(s8 & outbyChar);
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// Reads indwStreamSize bytes from the stream unless an EOF is encountered.
			///
			/// @param buffer for the output string.
			/// @param The amount of bytes to read into the buffer.
			//--------------------------------------------------------------------------------------------------
			virtual void Get(s8 * outpbyString, s32 indwStreamSize);
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// Reads indwStreamSize bytes from the stream unless the character specifed by inbyDelim is 
			/// encountered
			///
			/// @param buffer for the output string.
			/// @param The amount of bytes to read into the buffer.
			/// @param The delimiting character.
			//--------------------------------------------------------------------------------------------------
			virtual void Get(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim);
			//--------------------------------------------------------------------------------------------------
			/// Get Line
			///
			/// Reads a single line into a std string.
			///
			/// @param OUT: output string.
			//--------------------------------------------------------------------------------------------------
			virtual void GetLine(std::string &outstrString);
			//--------------------------------------------------------------------------------------------------
			/// Get All
			///
			/// Reads the entire file into a std string.
			///
			/// @param OUT: output string.
			//--------------------------------------------------------------------------------------------------
			virtual void GetAll(std::string &outstrString);
			//--------------------------------------------------------------------------------------------------
			/// Get
			///
			/// Reads into a std string stream.
			///
			/// @param OUT: output string.
			//--------------------------------------------------------------------------------------------------
			virtual void Get(std::stringstream &outstrStringStream);
			//--------------------------------------------------------------------------------------------------
			/// Get Line
			///
			/// Reads indwStreamSize bytes from the stream unless a return character is encountered.
			///
			/// @param buffer for the output string.
			/// @param The amount of bytes to read into the buffer.
			//--------------------------------------------------------------------------------------------------
			virtual void GetLine(s8 * outpbyString, s32 indwStreamSize);
			//--------------------------------------------------------------------------------------------------
			/// Get Line
			///
			/// Reads indwStreamSize bytes from the stream unless the character specifed by inbyDelim is 
			/// encountered
			///
			/// @param buffer for the output string.
			/// @param The amount of bytes to read into the buffer.
			/// @param The delimiting character.
			//--------------------------------------------------------------------------------------------------
			virtual void GetLine(s8 * outpbyString, s32 indwStreamSize, s8 inbyDelim);
			//--------------------------------------------------------------------------------------------------
			/// Ignore
			///
			/// Reads indwStreamSize bytes from the stream unless the character specifed by inbyDelim is 
			/// encountered and the discards them.
			///
			/// @param The amount of bytes to read into the buffer.
			/// @param The delimiting character.
			//--------------------------------------------------------------------------------------------------
			virtual void Ignore(s32 indwStreamSize = 1, s8 inbyDelim = EOF);
			//--------------------------------------------------------------------------------------------------
			/// Peek
			///
			/// Reads the next byte in the stream and returns it as an integer, but doesn't seek to the next
			/// byte, leaving it remaining as the next byte.
			///
			/// @return the next byte as an integer.
			//--------------------------------------------------------------------------------------------------
			virtual s32 Peek();
			//--------------------------------------------------------------------------------------------------
			/// Read
			///
			/// Reads a block of data into the buffer on size specified by indwStreamSize
			///
			/// @param The buffer.
			/// @param the stream size.
			//--------------------------------------------------------------------------------------------------
			virtual void Read(s8* inpbyBuffer, s32 indwStreamSize);
			//--------------------------------------------------------------------------------------------------
			/// Read Some
			///
			/// Very similar to Read(), but this will stop if there are no more characters in the steam, even
			/// without hitting EOF.
			///
			/// @param the buffer.
			/// @param the stream size.
			//--------------------------------------------------------------------------------------------------
			virtual s32 ReadSome(s8* inpbyBuffer, s32 indwStreamSize);
			//--------------------------------------------------------------------------------------------------
			/// Put Back
			///
			/// Decriments the get pointer by one, and sets inbyChar as the next char to be read.
			///
			/// @param the next char.
			//--------------------------------------------------------------------------------------------------
			virtual void PutBack(s8 inbyChar);
			//--------------------------------------------------------------------------------------------------
			/// Unget
			///
			/// Decriments the get pointer by one.
			//--------------------------------------------------------------------------------------------------
			virtual void Unget();
			//--------------------------------------------------------------------------------------------------
			/// TellG
			///
			/// @return the absolute position of the get pointer.
			//--------------------------------------------------------------------------------------------------
			virtual s32 TellG();
			//--------------------------------------------------------------------------------------------------
			/// SeekG
			///
			/// Sets the position of the get pointer
			///
			/// @param the new position.
			//--------------------------------------------------------------------------------------------------
			virtual void SeekG(s32 indwPosition);
			//--------------------------------------------------------------------------------------------------
			/// SeekG
			///
			/// Sets the position of the get pointer, using the specified SEEK_DIR
			///
			/// @param the new position.
			/// @param the direction from which to seek.
			//--------------------------------------------------------------------------------------------------
			virtual void SeekG(s32 indwPosition, SEEK_DIR ineDir);
			//--------------------------------------------------------------------------------------------------
			/// Sync
			///
			/// Syncronises with the associated buffer. This effectively means that unread bytes are discarded.
			//--------------------------------------------------------------------------------------------------
			virtual s32 Sync();
			//--------------------------------------------------------------------------------------------------
			/// Put
			///
			/// Writes the specified character to the output.
			///
			/// @param the char to write.
			//--------------------------------------------------------------------------------------------------
			virtual void Put(s8 inbyChar);
			//--------------------------------------------------------------------------------------------------
			/// Write
			///
			/// Writes a block of data to the output.
			///
			/// @param the data buffer.
			/// @param the stream size.
			//--------------------------------------------------------------------------------------------------
			virtual void Write(s8* inpbyChar, s32 indwStreamSize);
			//--------------------------------------------------------------------------------------------------
			/// Write
			///
			/// Writes using a std:string
			///
			/// @param the string.
			//--------------------------------------------------------------------------------------------------
			virtual void Write(const std::string& _instrString);
            //--------------------------------------------------------------------------------------------------
			/// Write Encrypted
			///
			/// Writes a block of data to the output encrypted AES with a private key
			///
			/// @param the data buffer.
			/// @param the stream size.
            /// @param Private AES key
			//--------------------------------------------------------------------------------------------------
            void WriteEncrypted(s8* inpbyChar, s32 indwStreamSize, const std::string& instrPrivateKey);
			//--------------------------------------------------------------------------------------------------
			/// Write Encrypted
			///
			/// Writes encrypted using a std:string
			///
			/// @param the string.
            /// @param Private AES key
			//--------------------------------------------------------------------------------------------------
            void WriteEncrypted(const std::string& _instrString, const std::string& instrPrivateKey);
			//--------------------------------------------------------------------------------------------------
			/// TellP
			///
			/// @return the currently position of the put pointer
			//--------------------------------------------------------------------------------------------------
			virtual s32 TellP();
			//--------------------------------------------------------------------------------------------------
			/// SeekP
			///
			/// Sets the position of the put pointer
			///
			/// @param the new position.
			//--------------------------------------------------------------------------------------------------
			virtual void SeekP(s32 indwPosition);
			//--------------------------------------------------------------------------------------------------
			/// SeekP
			///
			/// Sets the position of the put pointer, using the specified SEEK_DIR
			///
			/// @param the new position.
			/// @param the direction from which to seek.
			//--------------------------------------------------------------------------------------------------
			virtual void SeekP(s32 indwPosition, SEEK_DIR ineDir);
			//--------------------------------------------------------------------------------------------------
			/// Flush
			///
			/// Synchronises the associated buffer with the stream.
			//--------------------------------------------------------------------------------------------------
			virtual void Flush();
            //--------------------------------------------------------------
            /// Get MD5 Checksum 
            ///
            /// Calculate the MD5 checksum of the file 
            ///
            /// @return MD5 checksum
            //--------------------------------------------------------------
            std::string GetMD5Checksum();
            //--------------------------------------------------------------
            /// Get SHA1 Checksum
            ///
            /// Calculate the SHA1 checksum of the file
            ///
            /// @param Type of checksum to report
            /// @return SHA1 checksum
            //--------------------------------------------------------------
            std::string GetSHA1Checksum(moFlo::Core::CSHA1::ReportType ineReportType);
            //--------------------------------------------------------------------------------------------------
			/// Open
			///
			/// Opens the filestream with the specified file mode.
			///
			/// @param The path to the file to be opened.
			/// @param The file mode with which the file should be opened.
			//--------------------------------------------------------------------------------------------------
			virtual void Open(const std::string& instrFilename, FILE_MODE ineMode);
            //--------------------------------------------------------------------------------------------------
			/// Open Encrypted
			///
			/// Opens the filestream to the encrypted file with the specified file mode. The file is then
            /// decrypted using the private key into memory. Subsequent actions are performed on the memory
            /// copy
			///
			/// @param The path to the file to be opened.
            /// @param Private AES key
			/// @param The file mode with which the file should be opened.
			//--------------------------------------------------------------------------------------------------
            void OpenEncrypted(const std::string& instrFilename, const std::string& instrPrivateKey, FILE_MODE ineMode);
            //--------------------------------------------------------------------------------------------------
			/// Constructor
			///
			/// This is defined protected so that only the FileSystem can create it.
			//--------------------------------------------------------------------------------------------------
			IFileStream(){};

		protected:

            //--------------------------------------------------------------------------------------------------
            /// Get File Mode
            //--------------------------------------------------------------------------------------------------
            std::ios_base::openmode GetFileMode() const;
            
            Core::FILE_MODE meFileMode;
        
			std::string mstrFilename;
			std::fstream mFileStream;
		};
	}
}


#endif