package com.example.tamagotchi

import android.content.Intent
import android.os.Bundle

import androidx.activity.compose.setContent

import android.util.Log
import android.widget.Toast

import com.google.android.material.bottomnavigation.BottomNavigationView
import androidx.appcompat.app.AppCompatActivity
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material.Text
import androidx.compose.material.TextButton
import androidx.compose.material3.Button
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.ComposeView
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.Font
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.navigation.NavController
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.example.tamagotchi.databinding.ActivityEggBinding
import com.example.tamagotchi.databinding.ActivityMainBinding
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.database.ktx.getValue
import com.google.firebase.ktx.Firebase

class EggActivity : AppCompatActivity() {

    private lateinit var binding: ActivityEggBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityEggBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val database = Firebase.database

        checkExistence(database) { crackedEgg ->
            // Use the value of crackedEgg here or perform any actions based on it
            if (crackedEgg) { // dragon already exists

                Log.d("FireBase", "Dragon Exists")
                // go to main activity where all dragon activities are
                val intent: Intent = Intent(this, MainActivity::class.java)
                startActivity(intent)
                finish() // remove this activity from the stack

            } else { // The dragon does not exist

                Log.d("FireBase", "Dragon DeezNuts")
                // stay in this activity so user can crack egg

                val composeView: ComposeView = binding.composeView

                composeView.setContent {

                    EggCrack()
                }
            }


        }
    }

    fun checkExistence(database: FirebaseDatabase, callback: (Boolean) -> Unit) {
        val dragonExists = database.getReference("dragonExists")

        dragonExists.addListenerForSingleValueEvent(object : ValueEventListener {

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

    @Preview
    @Composable
    fun EggCrack(){
        val configuration = LocalConfiguration.current
        val screenHeight = configuration.screenHeightDp.dp
        val screenWidth = configuration.screenWidthDp.dp
        var numberOfClicks = 1
        val youngSerif = FontFamily(
            Font(R.font.young_serif, FontWeight.Bold)
        )
        val ctx = LocalContext.current
        Column(modifier = Modifier
            .height(screenHeight)
            .width(screenWidth)
            .background(color = Color.White),
            verticalArrangement = Arrangement.Center,
            horizontalAlignment = Alignment.CenterHorizontally
        ){
            Text(text="Click the Egg to Crack", fontFamily = youngSerif, fontSize = 30.sp)
            TextButton( modifier = Modifier.padding(10.dp), onClick={

                // egg clicked
                if (numberOfClicks < 3) {
                    numberOfClicks++;
                    binding.textView.text = "CLICK AGAIN!"

                } else if (numberOfClicks == 3) {

                    // go to main activity where all dragon activities are
                    val intent: Intent = Intent(ctx, MainActivity::class.java)

                    // Initialize
                    val database = Firebase.database
                    val healthRef = database.getReference("health").setValue(5)
                    val boredomRef = database.getReference("boredomLevel").setValue(0)
                    val cleanlinessRef = database.getReference("cleanlinessLevel").setValue(5)
                    val dragonExists = database.getReference("dragonExists").setValue(true)
                    val hungerRef = database.getReference("hunger").setValue(0)
                    val mopRef = database.getReference("usedMop").setValue(false)
                    val ballRef = database.getReference("threwBall").setValue(false)
                    val foodRef = database.getReference("threwFood").setValue(false)

                    startActivity(intent)
                    finish() // remove this activity from the stack

                }

            }){
                Image(

                    modifier = Modifier.size(300.dp),
                    painter = painterResource(R.drawable.egg),
                    contentDescription = "Contact profile picture",
                )
            }
        }
    }


}