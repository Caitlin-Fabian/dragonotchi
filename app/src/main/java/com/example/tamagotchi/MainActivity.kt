package com.example.tamagotchi

import android.os.Bundle

import androidx.activity.compose.setContent

import android.util.Log

import com.google.android.material.bottomnavigation.BottomNavigationView
import androidx.appcompat.app.AppCompatActivity
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material.Text
import androidx.compose.material3.Button
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.example.tamagotchi.databinding.ActivityMainBinding
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.database.ktx.getValue
import com.google.firebase.ktx.Firebase

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val navView: BottomNavigationView = binding.navView

        val navController = findNavController(R.id.nav_host_fragment_activity_main)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.


        val appBarConfiguration = AppBarConfiguration(
            setOf(
                R.id.navigation_home, R.id.navigation_dashboard, R.id.navigation_notifications
            )
        )

        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)

        val database = Firebase.database

        checkExistence(database){crackedEgg ->
            // Use the value of crackedEgg here or perform any actions based on it
            if (crackedEgg) {
                // The dragon exists
                Log.d("FireBase", "Dragon Exists")

            } else {
                // The dragon does not exist
                Log.d("FireBase", "Dragon DeezNuts")
//                navController.navigate(R.id.navigation_egg)
            }


        }
    }

    fun checkExistence( database: FirebaseDatabase, callback: (Boolean) -> Unit) {
        val dragonExists = database.getReference("dragonExists")

        dragonExists.addListenerForSingleValueEvent(object: ValueEventListener {

            override fun onDataChange(snapshot: DataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                val value = snapshot.getValue<Boolean>()
                Log.d("FireBase", "Value is: " + value)
                callback(value ?: false)

            }

            override fun onCancelled(error: DatabaseError) {
                Log.w("FireBase", "Failed to read value.", error.toException())
                callback(false) // Handle the error by providing a default value

            }

        })


    }

//
//        // Write a message to the database
//        val database = Firebase.database
//        val healthRef = database.getReference("health")
//        val boredomRef = database.getReference("boredomLevel")
//        val cleanlinessRef = database.getReference("cleanlinessLevel")
//        val dragonExists = database.getReference("dragonExists")
//
//        dragonExists.setValue(false)
//        healthRef.setValue("5")
//        boredomRef.setValue("5")
//        cleanlinessRef.setValue("5")
//
//
//
//
//
//
//        // Read from the database
//        healthRef.addValueEventListener(object: ValueEventListener {
//
//            override fun onDataChange(snapshot: DataSnapshot) {
//                // This method is called once with the initial value and again
//                // whenever data at this location is updated.
//                val value = snapshot.getValue<String>()
//                Log.d("FireBase", "Value is: " + value)
//            }
//
//            override fun onCancelled(error: DatabaseError) {
//                Log.w("FireBase", "Failed to read value.", error.toException())
//            }
//
//        })
//
//        // Read from the database
//        cleanlinessRef.addValueEventListener(object: ValueEventListener {
//
//            override fun onDataChange(snapshot: DataSnapshot) {
//                // This method is called once with the initial value and again
//                // whenever data at this location is updated.
//                val value = snapshot.getValue<String>()
//                Log.d("FireBase", "Value is: " + value)
//            }
//
//            override fun onCancelled(error: DatabaseError) {
//                Log.w("FireBase", "Failed to read value.", error.toException())
//            }
//
//        })
//
//        // Read from the database
//        boredomRef.addValueEventListener(object: ValueEventListener {
//
//            override fun onDataChange(snapshot: DataSnapshot) {
//                // This method is called once with the initial value and again
//                // whenever data at this location is updated.
//                val value = snapshot.getValue<String>()
//                Log.d("FireBase", "Value is: " + value)
//            }
//
//            override fun onCancelled(error: DatabaseError) {
//                Log.w("FireBase", "Failed to read value.", error.toException())
//            }
//
//        })
//
//        // calling this activity's function to
//        // use ActionBar utility methods
//        val actionBar = supportActionBar
//        // providing title for the ActionBar
//        actionBar!!.title = ""
//
//        // providing subtitle for the ActionBar
//        actionBar.subtitle = ""
//
//        // adding icon in the ActionBar
//        //actionBar.setIcon()
//
//        // methods to display the icon in the ActionBar
//        actionBar.setDisplayUseLogoEnabled(true)
//        actionBar.setDisplayShowHomeEnabled(true)
//    }
}